##
## Project nobudget, 2022
##
## Author Francois Michaut
##
## Started on  Thu Feb 17 18:53:24 2022 Francois Michaut
## Last update Fri Feb 18 15:44:35 2022 Francois Michaut
##
## Makefile : Makefile of the project
##

CC			=	gcc
CFLAGS		+=	-W -Wall -Werror -Wextra
LDFLAGS 	+=	-lncurses

BUILD_DIR	?=	build
SRC_DIR 	?=	source
INCS_DIR	?=	include

SRC_EXT		=	c
SUFFIXES	+=	.d

SRCS		:=	$(shell find $(SRC_DIR) -name *.$(SRC_EXT))
OBJS		:=	$(SRCS:$(SRC_DIR)/%.$(SRC_EXT)=$(BUILD_DIR)/%.o)
DEPS		:=	$(OBJS:%.o=%.d)

CFLAGS		+=	-I $(INCS_DIR)
ifdef DEBUG
  CFLAGS	+=	-DDEBUG -g3
endif

NAME		=	nobudget

YELLOW		=	\033[0;103m
RED			=	\033[0;31m
GREEN		=	\033[0;32m
NC			=	\033[0m
GREY		=	\033[90m
BLUE		=	\033[0;94m
PURPLE		=	\033[0;95m
BG_COLOR	=	\033[46m####\033[m
IRED		=	\033[0;91m


all:	$(NAME)

$(NAME):	$(OBJS)
	@echo -e '${NC}${BG_COLOR}Libs: $(LDFLAGS)${NC}'
	@echo -e '${BG_COLOR}Flags: $(CFLAGS)${NC}'
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS) \
		&& echo -e '${BLUE}Create${NC}: ${YELLOW}${GREY}./$(NAME)${NC}'\
		|| (echo -e '${RED}[ FAILED ]${NC}: __Create__${GREY} ./$(NAME)${NC}' && exit 1)

${BUILD_DIR}/%.d: ${SRC_DIR}/%.c
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -MM -MT '$(patsubst ${SRC_DIR}/%.c,${BUILD_DIR}/%.o,$<)' $< -MF $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c  $(BUILD_DIR)/%.d
	@rm -f $@
	@mkdir -p $(shell dirname $@)
	@-$(CC) $(CFLAGS) -MMD -o $@ -c $< \
		&& echo -e '${GREEN} [ OK ]${NC} Build $<'\
		|| echo -e '${RED}[ FAILED ]${NC} __Build__ $<'
	@echo -ne '${NC}'

clean:
	@find . -name "*~" -delete -o -name "#*#" -delete
	@rm -rf ./build
	@rm -rf $(OBJS) $(DEPS)
	@rm -rf vgcore.*
	@rm -rf a.out
	@rm -rf $(NAME)
	@find . -name "*.d" -delete
	@echo -e '${BLUE}Clean${NC} : OK'

re:	clean	all

-include $(DEPS)

ifndef VERBOSE
MAKEFLAGS += --no-print-directory
endif

#A phony target should not be a prerequisite of a real target file;
#if it is, its recipe will be run every time make goes to update that file.
.PHONY:	all	$(NAME)	clean	re
