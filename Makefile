NAME		= webserv 
NAME_SAN	= webserv_san

CC			= c++

CFLAGS 		= -Wall -Werror -Wextra -g3 -std=c++98 -MD
CFLAGS_SAN 	= -Wall -Werror -Wextra -g3 -std=c++98 -MD -fsanitize=address

SRC_MAIN	= ./
SRC_DIR		= src/
SRC_DIR_UT	= src/utils/
SRC_DIR_SE  = src/services/
INC_DIR		= inc/
OBJ_DIR		= obj/
OBJ_DIR_SAN	= obj_san/

HEADERS		= ./$(INC_DIR) 
INCLUDES	= $(addprefix -I, $(HEADERS))

SRC			= $(wildcard $(SRC_MAIN)*.cpp) $(wildcard $(SRC_DIR)*.cpp) $(wildcard $(SRC_DIR_UT)*.cpp) $(wildcard $(SRC_DIR_SE)*.cpp)
OBJ			= $(addprefix $(OBJ_DIR), $(notdir $(SRC:.cpp=.o)))
OBJ_SAN		= $(addprefix $(OBJ_DIR_SAN), $(notdir $(SRC:.cpp=.o)))

all: $(NAME) load_gallery

$(NAME): $(OBJ) 
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo $(C_GREEN)"✅ CREATED: $(NAME)    // Compiled with: $(CC) $(CFLAGS) //\n"$(C_RESET)

$(OBJ_DIR)%.o: $(SRC_MAIN)%.cpp 
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp 
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)%.o: $(SRC_DIR_UT)%.cpp 
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)%.o: $(SRC_DIR_SE)%.cpp 
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

san: $(NAME_SAN)

$(NAME_SAN): $(OBJ_SAN) 
	$(CC) $(CFLAGS_SAN) $(OBJ_SAN) -o $(NAME_SAN)
	@echo $(C_GREEN)"✅ CREATED: $(NAME_SAN)    // Compiled with: $(CC) $(CFLAGS_SAN) //\n"$(C_RESET)

$(OBJ_DIR_SAN)%.o: $(SRC_MAIN)%.cpp 
	@mkdir -p $(OBJ_DIR_SAN)
	$(CC) $(CFLAGS_SAN) $(INCLUDES) -c $< -o $@

$(OBJ_DIR_SAN)%.o: $(SRC_DIR)%.cpp 
	@mkdir -p $(OBJ_DIR_SAN)
	$(CC) $(CFLAGS_SAN) $(INCLUDES) -c $< -o $@

$(OBJ_DIR_SAN)%.o: $(SRC_DIR_UT)%.cpp 
	@mkdir -p $(OBJ_DIR_SAN)
	$(CC) $(CFLAGS_SAN) $(INCLUDES) -c $< -o $@

$(OBJ_DIR_SAN)%.o: $(SRC_DIR_SE)%.cpp 
	@mkdir -p $(OBJ_DIR_SAN)
	$(CC) $(CFLAGS_SAN) $(INCLUDES) -c $< -o $@

load_gallery:
	@# Copy all assets from backup so that they can be deleted
	@rm -rf public/pictures/*
	@cp -a backup/. public/pictures

run: re load_gallery
	./webserv config/init.conf

run-san: fclean san load_gallery
	./webserv_san config/init.conf

leaks: re load_gallery
	valgrind --leak-check=full ./webserv config/init.conf

test:
	make -C tests run

tester:
	./42_tester/tester http://localhost:8081

-include $(OBJ_DIR)/*.d

.PHONY: all clean fclean re

C_RED		= "\033[31m"
C_GREEN		= "\033[32m"
C_YELLOW	= "\033[33m"
C_RESET		= "\033[0m"
