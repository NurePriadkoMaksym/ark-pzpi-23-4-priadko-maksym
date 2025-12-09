using PythonWiki.Dtos.Articles;

namespace PythonWiki.Services.Interfaces;

public interface IArticleService
{
    Task<ArticleResponse> CreateArticleAsync(int authorId, CreateArticleRequest request);
    Task DeleteArticleAsync(int articleId);
    Task AddLinkAsync(int fromArticleId, int toArticleId);
    Task RemoveLinkAsync(int fromArticleId, int toArticleId);
    Task<ArticleResponse> UpdateArticleAsync(int articleId, int editorId, UpdateArticleRequest request);
}