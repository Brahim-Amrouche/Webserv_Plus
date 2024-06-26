NAME = webserv

INCLUDES = -I./includes -I./includes/server -I./includes/helpers -I./includes/config  -I./includes/request -I./includes/response

FLAGS = -Wall -Wextra -Werror -std=c++98 ${INCLUDES} -fsanitize=address

SRCS = main.cpp \
./src/helpers/REQH.cpp \
./src/helpers/RESH.cpp \
./src/helpers/PH.cpp \
./src/helpers/Path.cpp \
./src/helpers/FT.cpp \
./src/config/TokenizeInput.cpp \
./src/config/ServerConfiguration.cpp \
./src/config/ConfigParser.cpp \
./src/config/main_config.cpp \
./src/response/Cgi.cpp \
./src/response/Response.cpp \
./src/response/File.cpp \
./src/server/Socket.cpp \
./src/server/Client.cpp \
./src/server/LoadBalancer.cpp \
./src/request/Headers.cpp \
./src/request/Body.cpp \
./src/request/Request.cpp

CC = c++

DEFAULT_ROOT := ${shell pwd}

OBJECT_FILES = $(patsubst %.cpp,%.o,$(SRCS))

%.o : %.cpp
	@$(CC) $(FLAGS) -DDEFAULT_ROOT='"${DEFAULT_ROOT}"' -c $^ -o $@

all : $(NAME)

$(NAME) : $(OBJECT_FILES)
	-mkdir ./tmp
	$(CC) $(FLAGS) $^ -o $(NAME) 

clean :
	@rm -rf ${OBJECT_FILES}

fclean : clean
	@rm -rf $(NAME)

re : fclean all

bonus : all

run : re
	clear
	./webserv

.PHONY : all clean fclean re run