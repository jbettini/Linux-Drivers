#include "utils.h"
#include <linux/slab.h>


void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list	*li;

	li = *alst;
	if (alst)
	{
		if (*alst)
		{
			while (li->next)
				li = li->next;
			li->next = new;
		}
		else {
			*alst = new;
		}
	}
}

void	ft_lstclear(t_list **alst, void (*del)(const void *))
{
	t_list	*li;

	li = *alst;
	if (alst)
	{
		if (*alst)
		{
			while (li)
			{
				li = li->next;
				(*del)((*alst)->content);
				kfree((void *)*alst);
				*alst = 0;
				*alst = li;
			}
		}
	}
}

t_list	*ft_lstnew(void *content)
{
	t_list	*li;

	li = kmalloc(sizeof(t_list), GFP_KERNEL);
	if (!li)
		return (NULL);
	li->content = content;
	li->next = NULL;
	li->prev = NULL;
	return (li);
}

void	ft_lstadd_front(t_list **alst, t_list *start)
{
	if (alst)
	{
		if (*alst)
			start->next = *alst;
		*alst = start;
	}
}

char	*ft_strdup(const char *src)
{
	char	*str;
	size_t	i;

	i = 0;
	str = kmalloc(sizeof(char) * ft_strlen(src) + 1, GFP_KERNEL);
	if (!str)
		return (NULL);
	while (src[i])
	{
		str[i] = src[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

size_t ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}