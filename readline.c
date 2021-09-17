#include "minishell.h"

extern t_ext	g_var;

static void	input_is_null(void)
{
	printf("%c[1A", 27);
	printf("%c[1000D", 27);
	printf("%sexit\n", make_prompt());
	printf(COLOR_RESET);
	tcsetattr(STDIN_FILENO, TCSANOW, &g_var.restore);
	exit(0);
}

/* t_cmd *c 동적할당 해주는 부분 */
static t_cmd	*malloc_c(char *input)
{
	t_cmd	*c;
	int		size;

	size = count_cmd(input);
	g_var.first_input_size = size;
	if (size == ERROR)
	{
		printf("minishell : error\n");
		g_var.qmark = 258;
		return (NULL);
	}
	c = (t_cmd *)malloc(sizeof(t_cmd) * size);
	ft_memset(c, 0, sizeof(t_cmd) * size);
	return (c);
}

char	*make_prompt(void)
{
	static char	prompt[20];

	ft_memset(&prompt[0], 0, 20);
	prompt[0] = 'm';
	prompt[1] = 'i';
	prompt[2] = 'n';
	prompt[3] = 'i';
	prompt[4] = 's';
	prompt[5] = 'h';
	prompt[6] = 'e';
	prompt[7] = 'l';
	prompt[8] = 'l';
	prompt[9] = '$';
	prompt[10] = ' ';
	prompt[11] = '\0';
	return (prompt);
}

static void	freee_c(t_cmd **c)
{
	int	i;
	int	j;

	j = 0;
	while (j < g_var.first_input_size && &(c[0][j]))
	{
		i = -1;
		if (c[0][j].cmd)
		{
			while ((c[0][j].cmd[++i]))
				free((c[0][j].cmd[i]));
			free((c[0][j].cmd[i]));
			free((c[0][j].cmd));
		}
		j++;
	}
	if (*c)
		free(*c);
}

int	start_read(void)
{
	t_cmd	*c;
	char	*input;
	char	*tmp;

	while (1)
	{
		input = readline(make_prompt());
		tmp = input;
		if (input == NULL)
			input_is_null();
		if (all_space(&input))
			continue ;
		add_history(input);
		c = malloc_c(input);
		if (g_var_set() && c && fill_cmd(&c, &input) && ++g_var.writing)
		{	
			g_var.qmark = exec_cmd(c);
			g_var.writing = 0;
		}
		freee_c(&c);
		if (g_var.fre)
			free(g_var.fre);
		free(tmp);
	}
	return (0);
}
