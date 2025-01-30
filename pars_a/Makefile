

NAME 			= webserv
HEADER_FILES	= ConfigFileParser.hpp \
				  Parser.hpp
SOURCE_FILES	= main.cpp\
				  ConfigFileParser.cpp \
				  getFileContent.cpp\
				  check_file_type_path.cpp\
				  pars_utils.cpp directive_handlers.cpp

OBJECT_FILES	= $(SOURCE_FILES:.cpp=.o)

CPP 	= c++
CFLAGS 	= -Wall -Wextra -Werror #-fsanitize=address
STD		= -std=c++98
RM 		= rm -rf

all: $(NAME)

$(NAME): $(OBJECT_FILES)
	$(CPP) $(CFLAGS) $(STD) $(OBJECT_FILES) -o $(NAME)

$(OBJECT_FILES): $(SOURCE_FILES)
	$(CPP) $(CFLAGS) $(STD) -c $(SOURCE_FILES)

clean:
	$(RM) $(OBJECT_FILES)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
