NAME = webserv

INCLUDES = -I./includes -I./includes/server -I./includes/helpers -I./includes/config  -I./includes/request -I./includes/response

FLAGS = -Wall -Wextra -Werror -std=c++98 ${INCLUDES} -g -fsanitize=address,undefined

SRCS = ${wildcard ./src/**/*.cpp} ${wildcard ./src/*.cpp} ${wildcard ./*.cpp}

CC = c++

OBJECT_FILES = $(patsubst %.cpp,%.o,$(SRCS))

%.o : %.cpp
	@$(CC) $(FLAGS) -c $^ -o $@

all : $(NAME)

$(NAME) : $(OBJECT_FILES)
	@$(CC) $(FLAGS) $^ -o $(NAME)

clean :
	@rm -rf ${OBJECT_FILES}

fclean : clean
	@rm -rf $(NAME)
	rm -rf ${wildcard ./tmp/*.tmp} ${wildcard ./tmp/*.res}

re : fclean all

run : re
	clear
	./webserv

.PHONY : all clean fclean re