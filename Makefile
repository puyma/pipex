# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/21 20:00:01 by mpuig-ma          #+#    #+#              #
#    Updated: 2023/05/23 18:17:20 by mpuig-ma         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

AUTHOR		?=	mpuig-ma

BUILD_DIR	:=	build
SRC_DIR		:=	src
LIBFT_DIR	:=	$(SRC_DIR)/libft

LIBFT		:=	$(LIBFT_DIR)/libft.a
NAME		:=	pipex

CC			:=	gcc
CFLAGS		:=	-Wall -Werror -Wextra
CFLAGS		+=	-MMD
LFLAGS		:=	-L./$(LIBFT_DIR) -lft
DEBUG		:=	-g -fsanitize='address,undefined'
CFLAGS		+=	$(DEBUG)
INC			:=	-I$(SRC_DIR) -I$(LIBFT_DIR)/src
RM			:=	rm -rf

OS			:=	$(shell uname)
#ifeq ($(OS),Linux)
#endif

NOSTYLE		:=	\033[0m
STYLE		:=	\033[0;32m

SRC_FILES	:=	$(SRC_DIR)/main.c
OBJ_FILES	=	$(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(notdir $(basename $(SRC_FILES)))))
DEP_FILES	=	$(addprefix $(BUILD_DIR)/, $(addsuffix .d, $(notdir $(basename $(SRC_FILES)))))

.PHONY: clean fclean re all debug

$(NAME): $(LIBFT) $(OBJ_FILES) $(DEP_FILES) $(SRC_DIR)/$(NAME).h
	$(CC) $(INC) $(CFLAGS) $(LFLAGS) $(OBJ_FILES) -o $(NAME)
	@echo "Built $(STYLE)$(basename $@)$(NOSTYLE)"

$(LIBFT):
	make -C $(dir $@)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(BUILD_DIR)/
	make fclean -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME)*
	$(RM) bonus*

re: fclean
	$(MAKE)

all: $(NAME)
