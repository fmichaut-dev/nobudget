##
## Project nobudget, 2022
##
## Author Francois Michaut
##
## Started on  Thu Feb 17 18:53:24 2022 Francois Michaut
## Last update Sun Mar 27 00:51:17 2022 Francois Michaut
##
## Makefile : Makefile of the project
##

CC			=	g++
CFLAGS		+=	-W -Wall -Werror -Wextra
LDFLAGS 	+=	-lcpp-ncurses

BUILD_DIR	?=	build
SRC_DIR 	?=	source
INCS_DIR	?=	include
LIB_DIR		?=	lib

LIBS		:=	$(shell find $(LIB_DIR) -maxdepth 1 -type d ! -path "*/libs" ! -path "*/include" ! -path "$(LIB_DIR)" -exec basename {} \;)

SRC_EXT		=	cpp
SUFFIXES	+=	.d

SRCS		:=	$(shell find $(SRC_DIR) -name *.$(SRC_EXT))
OBJS		:=	$(SRCS:$(SRC_DIR)/%.$(SRC_EXT)=$(BUILD_DIR)/%.o)
DEPS		:=	$(OBJS:%.o=%.d)

LDFLAGS		+=	-L $(LIB_DIR)/libs
CFLAGS		+=	-I $(INCS_DIR) -I $(LIB_DIR)/include/
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

$(NAME):	$(LIB_DIR)/libs	$(OBJS)
	@echo -e '${NC}${BG_COLOR}Libs: $(LDFLAGS)${NC}'
	@echo -e '${BG_COLOR}Flags: $(CFLAGS)${NC}'
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS) \
		&& echo -e '${BLUE}Create${NC}: ${YELLOW}${GREY}./$(NAME)${NC}'\
		|| (echo -e '${RED}[ FAILED ]${NC}: __Create__${GREY} ./$(NAME)${NC}' && exit 1)

$(BUILD_DIR)/%.o:	$(SRC_DIR)/%.${SRC_EXT}	$(BUILD_DIR)/%.d	$(LIB_DIR)/include
	@rm -f $@
	@mkdir -p $(shell dirname $@)
	@-$(CC) $(CFLAGS) -MMD -o $@ -c $< \
		&& echo -e '${GREEN} [ OK ]${NC} Build $<'\
		|| echo -e '${RED}[ FAILED ]${NC} __Build__ $<'
	@echo -ne '${NC}'

${BUILD_DIR}/%.d: ${SRC_DIR}/%.${SRC_EXT}	$(LIB_DIR)/include
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -MM -MT '$(patsubst ${SRC_DIR}/%.${SRC_EXT},${BUILD_DIR}/%.o,$<)' $< -MF $@

$(LIB_DIR)/include:
	@mkdir -p $(LIB_DIR)/include
	@$(foreach lib,$(LIBS),mkdir -p "$(LIB_DIR)/include/$(lib)";)
	@$(foreach lib,$(LIBS),cp -r "$(LIB_DIR)/$(lib)"/include/* "$(LIB_DIR)/include/$(lib)";)

$(LIB_DIR)/libs:
	@echo -e '${NC}${BLUE}Building libs${NC}... ($(LIBS))'
	@mkdir -p $(LIB_DIR)/libs
	@$(foreach lib,$(LIBS),make -C "$(LIB_DIR)/$(lib)";)
	@$(foreach lib,$(LIBS),ln -fsr "$(LIB_DIR)/$(lib)"/*.so $(LIB_DIR)/libs/;)

clean:
	@echo -e '$(NC)$(BLUE)Cleaning$(NC)...'
	@make -C $(LIB_DIR)/cpp-ncurses clean > /dev/null
	@find . -name "*~" -delete -o -name "#*#" -delete
	@rm -rf ./build
	@rm -rf $(OBJS) $(DEPS)
	@rm -rf vgcore.*
	@rm -rf $(NAME) a.out
	@rm -rf $(LIB_DIR)/include $(LIB_DIR)/libs
	@find . -name "*.d" -delete
	@echo -e '${BLUE}Clean${NC} : OK'

re:	clean
	@make --no-print-directory all

$(shell git submodule update --init --recursive)

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

ifndef VERBOSE
MAKEFLAGS += --no-print-directory
endif

#A phony target should not be a prerequisite of a real target file;
#if it is, its recipe will be run every time make goes to update that file.
.PHONY:	all	$(NAME)	clean	re
