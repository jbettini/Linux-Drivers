#pragma once


typedef struct s_list
{
	void			*content;
	struct s_list	*next;
	struct s_list	*prev;
}				t_list;

t_list	*ft_lstnew(void *content);
void	ft_lstclear(t_list **alst, void (*del)(const void *));
void	ft_lstadd_back(t_list **alst, t_list *new);
void	ft_lstadd_front(t_list **alst, t_list *start);
char	*ft_strdup(const char *src);
unsigned long int	ft_strlen(const char *str);