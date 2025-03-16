#include "libft.h"

typedef struct s_garb
{
	void	*obj;
	void	(*f)(void *);
} t_garb;

void clear_garbage(t_list **head)
{
	t_list	*tmp;
	t_list	*tmp_next;
	t_garb	*bin;

	tmp = *head;
	while (tmp)
	{
		bin = tmp->content;
		tmp_next = tmp->next;
		if(bin->f)
			bin->f(bin->obj);
		free(bin);
		free(tmp);
		tmp = tmp_next;
	}
	*head = NULL;
	
}

void	*add_garbage(void *pt, void (*free_pt)(void *), t_list **head)
{
	t_garb	*new_garb;
	t_list	*new_node;

	new_garb = malloc(sizeof(t_garb));
	if (new_garb)
		new_node = ft_lstnew(new_garb);
	if (!new_garb || !new_node)
	{
		if (new_garb)
			free(new_garb);
		free_pt(pt);
		clear_garbage(head);
		return (NULL);
	}
	new_garb->obj = pt;
	new_garb->f = free_pt;
	ft_lstadd_back(head, new_node);
	return(new_node);
}
/* 
int main()
{
	void	**new_malloc;
	t_list	*bin;
	int		i = 0;

	bin = NULL;
	i = 0;
	new_malloc = ft_calloc(sizeof(char *), 51);
	while (i < 50)
		new_malloc[i++] = malloc(sizeof(char) * 50);
	if (!add_garbage(new_malloc, free_the_mallocs, &bin))
		exit(1);
	new_malloc = ft_calloc(sizeof(char *), 51);
	i = 0;
	while (i < 50)
		new_malloc[i++] = malloc(sizeof(char) * 50);
	if (!add_garbage(new_malloc, free_the_mallocs, &bin))
		exit(1);
	i = 0;
	new_malloc = ft_calloc(sizeof(char *), 51);
	while (i < 50)
		new_malloc[i++] = malloc(sizeof(char) * 50);
	if (!add_garbage(new_malloc, free_the_mallocs, &bin))
		exit(1);
	new_malloc = ft_calloc(sizeof(char *), 51);
	i = 0;
	while (i < 50)
		new_malloc[i++] = malloc(sizeof(char) * 50);
	if (!add_garbage(new_malloc, free_the_mallocs, &bin))
		exit(1);
	
	i = 0;
	new_malloc = ft_calloc(sizeof(char *), 51);
	while (i < 50)
		new_malloc[i++] = malloc(sizeof(char) * 50);
	if (!add_garbage(new_malloc, free_the_mallocs, &bin))
		exit(1);
	new_malloc = ft_calloc(sizeof(char *), 51);
	i = 0;
	while (i < 50)
		new_malloc[i++] = malloc(sizeof(char) * 50);
	if (!add_garbage(new_malloc, free_the_mallocs, &bin))
		exit(1);
	
	i = 0;
	new_malloc = ft_calloc(sizeof(char *), 51);
	while (i < 50)
		new_malloc[i++] = malloc(sizeof(char) * 50);
	if (!add_garbage(new_malloc, free_the_mallocs, &bin))
		exit(1);
	new_malloc = ft_calloc(sizeof(char *), 51);
	i = 0;
	while (i < 50)
		new_malloc[i++] = malloc(sizeof(char) * 50);
	if (!add_garbage(new_malloc, free_the_mallocs, &bin))
		exit(1);
	clear_garbage(&bin);

	return (0);
} */