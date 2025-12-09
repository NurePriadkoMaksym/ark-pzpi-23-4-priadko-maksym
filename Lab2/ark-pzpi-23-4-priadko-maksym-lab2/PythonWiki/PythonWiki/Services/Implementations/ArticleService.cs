using Microsoft.EntityFrameworkCore;
using PythonWiki.Dtos.Articles;
using PythonWiki.Models;
using PythonWiki.Persistence.DbContext;
using PythonWiki.Services.Interfaces;

namespace PythonWiki.Services.Implementations;

public class ArticleService : IArticleService
{
    private readonly PythonWikiDbContext _db;

    public ArticleService(PythonWikiDbContext db)
    {
        _db = db;
    }

    public async Task<ArticleResponse> CreateArticleAsync(int authorId, CreateArticleRequest request)
    {
        var article = new Article
        {
            Title = request.Title,
            Content = request.Content,
            XPReward = request.XPReward,
            XPRequired = request.XPRequired,
            AuthorId = authorId,
            IsLocked = true 
        };

        _db.Articles.Add(article);
        await _db.SaveChangesAsync();
        if (request.LinkToArticleIds != null)
        {
            foreach (var toId in request.LinkToArticleIds)
            {
                var link = new ArticleLink
                {
                    FromArticleId = article.Id,
                    ToArticleId = toId
                };

                _db.ArticleLinks.Add(link);
            }

            await _db.SaveChangesAsync();
        }

        return new ArticleResponse
        {
            Id = article.Id,
            Title = article.Title,
            Content = article.Content,
            XPReward = article.XPReward,
            XPRequired = article.XPRequired,
            IsLocked = article.IsLocked,
            AuthorId = article.AuthorId
        };
    }
    public async Task DeleteArticleAsync(int articleId)
    {
        var article = await _db.Articles
            .Include(a => a.OutgoingLinks)
            .Include(a => a.IncomingLinks)
            .Include(a => a.CourseArticles)
            .Include(a => a.UserProgresses)
            .FirstOrDefaultAsync(a => a.Id == articleId);

        if (article == null)
            throw new InvalidOperationException("Article not found.");
        if (article.OutgoingLinks.Any())
            _db.ArticleLinks.RemoveRange(article.OutgoingLinks);

        if (article.IncomingLinks.Any())
            _db.ArticleLinks.RemoveRange(article.IncomingLinks);

        if (article.CourseArticles.Any())
            _db.CourseArticles.RemoveRange(article.CourseArticles);
        if (article.UserProgresses.Any())
            _db.UserArticleProgresses.RemoveRange(article.UserProgresses);

        _db.Articles.Remove(article);

        await _db.SaveChangesAsync();
    }
    public async Task AddLinkAsync(int fromArticleId, int toArticleId)
    {
        if (fromArticleId == toArticleId)
            throw new InvalidOperationException("Cannot link an article to itself.");

        var from = await _db.Articles.FindAsync(fromArticleId);
        var to = await _db.Articles.FindAsync(toArticleId);

        if (from == null || to == null)
            throw new InvalidOperationException("One or both article IDs do not exist.");

        var exists = await _db.ArticleLinks
            .AnyAsync(l => l.FromArticleId == fromArticleId && l.ToArticleId == toArticleId);

        if (exists)
            return;

        _db.ArticleLinks.Add(new ArticleLink
        {
            FromArticleId = fromArticleId,
            ToArticleId = toArticleId
        });

        await _db.SaveChangesAsync();
    }

    public async Task RemoveLinkAsync(int fromArticleId, int toArticleId)
    {
        var link = await _db.ArticleLinks
            .FirstOrDefaultAsync(l => l.FromArticleId == fromArticleId && l.ToArticleId == toArticleId);

        if (link == null)
            throw new InvalidOperationException("Link does not exist.");

        _db.ArticleLinks.Remove(link);
        await _db.SaveChangesAsync();
    }
    public async Task<ArticleResponse> UpdateArticleAsync(int articleId, int editorId, UpdateArticleRequest request)
    {
        var article = await _db.Articles
            .Include(a => a.Author)
            .FirstOrDefaultAsync(a => a.Id == articleId);

        if (article == null)
            throw new InvalidOperationException("Article not found.");

        var user = await _db.Users.Include(u => u.Role).FirstAsync(u => u.Id == editorId);

        bool isAdmin = user.Role.Name == "Admin";
        bool isCreator = user.Role.Name == "Creator";
        bool isEditor = user.Role.Name == "Editor";
        bool isAuthor = article.AuthorId == editorId;

        if (!(isAdmin || isCreator || isEditor || isAuthor))
            throw new UnauthorizedAccessException("You are not allowed to edit this article.");

        article.Title = request.Title;
        article.Content = request.Content;
        article.XPReward = request.XPReward;
        article.XPRequired = request.XPRequired;

        if (request.IsLocked.HasValue)
            article.IsLocked = request.IsLocked.Value;

        await _db.SaveChangesAsync();

        return new ArticleResponse
        {
            Id = article.Id,
            Title = article.Title,
            Content = article.Content,
            XPReward = article.XPReward,
            XPRequired = article.XPRequired,
            IsLocked = article.IsLocked,
            AuthorId = article.AuthorId
        };
    }



}
