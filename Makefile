# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/13 14:40:15 by aelkheta          #+#    #+#              #
#    Updated: 2024/08/04 13:07:07 by aelkheta         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address
RLFLAG = -lreadline
SRCDIR = ./srcs/parsing_src/
BLTDIR = ./srcs/builtin_src/
EXECDIR = ./exec

OBJDIR = ./objs


SRC = 	$(SRCDIR)minishell.c\
		$(SRCDIR)cleanup.c\
		$(SRCDIR)expander_utiles.c\
		$(SRCDIR)parsing.c\
		$(SRCDIR)execution.c\
		$(SRCDIR)ft_split_str.c\
		$(SRCDIR)mini_utiles1.c\
		$(SRCDIR)parsing_utiles.c\
		$(SRCDIR)tokenizer.c\
		$(SRCDIR)expander.c\
		$(SRCDIR)lexer.c\
		$(SRCDIR)mini_utiles2.c\
		$(SRCDIR)print_functions.c\
		$(BLTDIR)built_in_cmd1.c\
		$(BLTDIR)built_in_cmd2.c\
		$(BLTDIR)built_in_cmd3.c\
		$(BLTDIR)built_in_utiles1.c\
		$(BLTDIR)built_in_utiles2.c\
		$(EXECDIR)/allcmd.c\
		$(EXECDIR)/error.c\
		$(EXECDIR)/excute.c\
		$(EXECDIR)/here_doc.c\
		$(EXECDIR)/link_list.c\
		$(EXECDIR)/main.c\
		$(EXECDIR)/strjoin.c\
		$(EXECDIR)/one_cmd.c\
		$(EXECDIR)/open_file.c\

OBJ = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC)) # for pathern substitution // $(patsubst pattern,replacement,text)

LIBFT = ./libraries/libft/libft.a

all: $(NAME) clean

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@echo "Building..."
	@$(CC) $(CFLAGS) $(OBJ) $(RLFLAG) $(LIBFT) -o $@
	@echo "Building done"

clean:
	@echo "Removing object files..."
	@rm -rf $(OBJDIR)/*.o
	@echo "Removing object files done"

fclean: clean
	@echo "Removing program name..."
	@rm -f $(NAME)
	@echo "Removing program name done"

re: fclean all

.PHONY: all clean fclean re
