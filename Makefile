# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/13 14:40:15 by aelkheta          #+#    #+#              #
#    Updated: 2024/08/09 12:51:53 by aelkheta         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 #-fsanitize=address
LIBFT = ./libraries/libft/libft.a
SRCDIR = ./srcs/parsing_src
BLTDIR = ./srcs/builtin_src
EXEDIR = ./srcs/exec
OBJDIR = ./objs
RLFLAG = -lreadline

SRC_P = $(SRCDIR)/minishell.c\
		$(SRCDIR)/cleanup.c\
		$(SRCDIR)/parsing.c\
		$(SRCDIR)/parsing_utiles.c\
		$(SRCDIR)/ft_split_str.c\
		$(SRCDIR)/mini_utiles1.c\
		$(SRCDIR)/mini_utiles2.c\
		$(SRCDIR)/mini_utiles3.c\
		$(SRCDIR)/exp_utiles1.c\
		$(SRCDIR)/exp_utiles2.c\
		$(SRCDIR)/exp_utiles3.c\
		$(SRCDIR)/exp_utiles4.c\
		$(SRCDIR)/tokenizer.c\
		$(SRCDIR)/expander.c\
		$(SRCDIR)/lexer.c\
		$(SRCDIR)/print_functions.c\

SRC_B =	$(BLTDIR)/built_in_.c\
		$(BLTDIR)/echo_.c\
		$(BLTDIR)/exit_.c\
		$(BLTDIR)/export_utls.c\
		$(BLTDIR)/cd_.c\
		$(BLTDIR)/env_.c\
		$(BLTDIR)/export_.c\
		$(BLTDIR)/pwd_unset.c\

SRC_E =	$(EXEDIR)/allcmd.c\
		$(EXEDIR)/error.c\
		$(EXEDIR)/excute.c\
		$(EXEDIR)/here_doc.c\
		$(EXEDIR)/link_list.c\
		$(EXEDIR)/main.c\
		$(EXEDIR)/strjoin.c\
		$(EXEDIR)/one_cmd.c\
		$(EXEDIR)/open_file.c\
		$(EXEDIR)/utillis.c\

OBJ_P = $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SRC_P))
OBJ_B = $(patsubst $(BLTDIR)%.c, $(OBJDIR)%.o, $(SRC_B))
OBJ_E = $(patsubst $(EXEDIR)%.c, $(OBJDIR)%.o, $(SRC_E))

all: $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(BLTDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(EXEDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@echo "Compiling libft..."
	@make -C ./libraries/libft
	@make bonus -C ./libraries/libft
	@echo "Compiling libft done"

$(NAME): $(LIBFT) $(OBJ_P) $(OBJ_B) $(OBJ_E)
	@echo "Building..."
	@$(CC) $(CFLAGS) $(OBJ_P) $(OBJ_B) $(OBJ_E) $(RLFLAG) $(LIBFT) -o $@
	@echo "Building done"

clean:
	@echo "Removing libft obj files..."
	@make clean -C ./libraries/libft
	@echo "Removing object files..."
	@rm -rf $(OBJDIR)/*.o
	@echo "Removing object files done"

fclean: clean
	@echo "Cleaning libft..."
	@make fclean -C ./libraries/libft
	@echo "Cleaning libft done"
	@echo "Removing program name..."
	@rm -f $(NAME)
	@echo "Removing program name done"

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: