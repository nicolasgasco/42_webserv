# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dgerwig- <dgerwig-@student.42urduli>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/12 17:44:11 by dgerwig-          #+#    #+#              #
#    Updated: 2023/02/13 19:41:16 by dgerwig-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= tests 

CC			= c++

CFLAGS 		= -std=c++11

SRC_TEST	= ./*
SRC_DIR		= ../src/
SRC_DIR_UT	= ../src/utils/
INC_DIR		= ../inc/
OBJ_DIR		= ../obj/

HEADERS		= ./$(INC_DIR) 
INCLUDES	= $(addprefix -I, $(HEADERS))

SRC			= $(wildcard $(SRC_TEST)*.cpp)  $(wildcard $(SRC_DIR)*.cpp) $(wildcard $(SRC_DIR_UT)*.cpp)  
OBJ			= $(addprefix $(OBJ_DIR), $(notdir $(SRC:.cpp=.o)))

all: $(NAME)

$(NAME): $(OBJ) 
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo $(C_GREEN)"✅ CREATED: $(NAME)    // Compiled with: $(CC) $(CFLAGS) //\n"$(C_RESET)

$(OBJ_DIR)%.o: $(SRC_TEST)%.cpp 
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp 
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)%.o: $(SRC_DIR_UT)%.cpp 
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(OBJ_DIR_SAN)
	@rm -rf *.dSYM
	@echo $(C_RED) "🗑  REMOVED: OBJECT FILES in $(NAME)\n" $(C_RESET)

fclean: clean
	rm -rf $(NAME)
	rm -rf $(NAME_SAN)
	@echo $(C_RED) "🗑  REMOVED: $(NAME)\n" $(C_RESET)

re:	fclean all





run: re 
	bash run_tests.sh


-include $(OBJ_DIR)/*.d

.PHONY: all clean fclean re

C_RED		= "\033[31m"
C_GREEN		= "\033[32m"
C_YELLOW	= "\033[33m"
C_RESET		= "\033[0m"
