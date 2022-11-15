#include "test.h"

t_token	*new_token(void)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	new->type = 0;
	new->cmd = 0;
	new->line = 0;
	new->next = 0;
	return (new);
}

int	sep_len(char *line)
{
	int	i;
	int	len;
	int	plag;

	plag = 0;
	len = ft_strlen(line);
	i = 0;
	while (*line)
	{
		if (*line != '<' && *line != '>' && *line != ' ')
			plag = 1;
		if (*line == ' ' && plag == 1)
			return (i);
		i++;
		line++;
	}
	return (i);
}

int	ft_sep(char *line)
{
	int	i;
	int	len;
	int	plag;

	i = 0;
	if (*line == '|' || *line == '&' || *line == ';')
	{
		if (*(line + 1) != 0 && (*(line + 1) == '&' || *(line + 1) == '|'))
			return (2);
		return (1);
	}
	while (*line)
	{
		if ((*line == '<' || *line == '>') && i != 0)
			return (i);
		else if ((*line == '<' || *line == '>') && i == 0)
			return (sep_len(line));
		else if (*line == '|' || *line == '&')
			return (i);
		i++;
		line++;
	}
	return (i);
}

void	create_a_token(t_token **token, char **line)
{
	t_token	*tail;
	t_token	*new;
	int		i;
	char	*tmp;

	i = 0;
	tmp = *line;
	while (**line == ' ' && **line != 0)
		(*line)++;
	new = new_token();
	i = ft_sep(*line);
	new->line = strndup(*line, i);
	(*line) += i;
	if (*token == 0)
		(*token) = new;
	else
	{
		tail = ft_lstlast(*token);
		tail->next = new;
	}
}

void	func(char *s)
{
	printf("list : '%s'\n", s);
}

void	ft_lstiter(t_token *lst, void (*f)(char *))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		f(lst->line);
		lst = lst->next;
	}	
}

t_oper_type	check_operator(char c)
{
	if (c == '|')
		return(TPIPE);
	else if (c == '&')
		return (TAND);
	else if (c == '<')
		return (TIN);
	else if (c == '>')
		return (TOUT);
	else if (c == ';')
		return(TSEMI);
	return (NO_TYPE);
}

// char	*check_type(t_token **token, char *line)
// {
// 	if (line[0] == '|')
// 	{
// 		if (line[1] == 0 || check_operator(line[1]) == 0)
// 			(*token)->type == TPIPE;
// 		else if (line[1] == '|' && check_operator(line[2]) == 0)
// 			(*token)->type == TOR;
// 	}
// 	if (line[0] == '&')
// 	{
// 		if (line[1] == 0 || check_operator(line[1]) == 0)
// 			(*token)->type == NO_TYPE;
// 		else if (line[1] == '&' && check_operator(line[2]) == 0)
// 			(*token)->type == TDAND;
// 	}
// 	if (line[0] == '<')
// 	{
// 		if (line[1] == 0 || check_operator(line[1]) == 0)
// 			(*token)->type == TIN;
// 		else if (line[1] == '<' && check_operator(line[2]) == 0)
// 			(*token)->type == TDOC;
// 	}
// 	if (line[0] == '>')
// 	{
// 		if (line[1] == 0 || check_operator(line[1]) == 0)
// 			(*token)->type == TOUT;
// 		else if (line[1] == '>' && check_operator(line[2]) == 0)
// 			(*token)->type == TADDOUT;
// 	}
// }

void	set_type(t_token **token, char oper, t_oper_type one, t_oper_type two)
{
	char	*line;

	line = (*token)->line;
	if ((*token)->line[0] == oper)
	{
		if (line[1] == 0 || check_operator(line[1]) == 0)
			(*token)->type == one;
		else if (line[1] == oper && check_operator(line[2]) == 0)
			(*token)->type == two;
	}
}
t_oper_type	check_type(t_token **token)
{
	set_type(token, '|', TPIPE, TOR);
	set_type(token, '&', NO_TYPE, TDAND);
	set_type(token, '<', TIN, TDOC);
	set_type(token, '>', TOUT, TADDOUT);
	if ((*token)->type == NO_TYPE)
		throw_error(OPER_ERR);
}

void	set_type_remove_operator(t_token **token)
{
	if (check_operator((*token)->line[0]))
	{
		check_type(token);
	}
	else
	{
		연산자일경우 연사자 따옴표처리 및 이중배열에 넣어준다
	}
}

int main()
{
	char *tmp = "         <   e ls -al -al <>          b <<c <<a >Q >D >V >BA >DBF ||& < Makefile  | wc -l | <b cat >   out >c && ls || ls";
	char *line;
	t_token	*token;

	token = 0;
	line = ft_strdup(tmp);
	int i = 0;

	tmp = line;
	while (*line)
		create_a_token(&token, &line);
	free(tmp);

	t_token *temp;
	temp = token;
	while (temp)
	{
		set_type_remove_operator(&temp);
		temp = temp->next;
	}
	ft_lstiter(token, func);
}