# -------- Printing Utils -------- #

SILENT_FLAG := $(findstring s, $(word 1, $(MAKEFLAGS)))

ifeq ($(SILENT_FLAG), s)
	OUTPUT_REDIRECT = >/dev/null
else
	OUTPUT_REDIRECT =
endif

# -------- Directories -------- #

SRC_DIR := $(CURDIR)/src
INC_DIR := $(CURDIR)/include
OBJ_DIR := $(CURDIR)/objs
DEP_DIR := $(CURDIR)/deps
TMP_DIR := $(CURDIR)/tmp
LIB_DIR := $(CURDIR)/lib

# -------- Library Name -------- #

LIB_NAME := logger.so

# -------- Input Files -------- #

SRCS = $(SRC_DIR)/logger.c

# -------- Binary File Structs -------- #

OBJ_FILE_STRUCT = $(dir $(SRCS:$(SRC_DIR)/%=$(OBJ_DIR)/%))
DEP_FILE_STRUCT = $(dir $(SRCS:$(SRC_DIR)/%=$(DEP_DIR)/%))

# -------- Output Files --------- #

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# -------- Dependency Files -------- #

DEPS  = $(SRCS:$(SRC_DIR)/%.c=$(DEP_DIR)/%.d)
DEPS += $(DEP_DIR)/$(BIN_NAME).d

# -------- Compiler and Linker flags -------- #

CFLAGS  += -I$(INC_DIR)

# -------- Util Targets -------- #

all:
	$(MAKE) dirs
	$(MAKE) lib

dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_FILE_STRUCT)
	
	mkdir -p $(DEP_DIR)
	mkdir -p $(DEP_FILE_STRUCT)
	
	mkdir -p $(TMP_DIR)
	mkdir -p $(LIB_DIR)
	
clean:
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(LIB_DIR)/$(LIB_NAME)
	
clean_all:
	$(RM) -r $(OBJ_DIR)
	$(RM) -r $(DEP_DIR)
	$(RM) -r $(TMP_DIR)
	$(RM) -r $(LIB_DIR)
	
remake:
	$(MAKE) clean
	$(MAKE) all

remake_all:
	$(MAKE) clean_all
	$(MAKE) all

.PHONY: all dirs clean clean_all remake remake_all lib shaders run

# -------- Program Targets -------- #

lib: $(LIB_DIR)/$(LIB_NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	set -e; \
	$(RM) $(DEP_DIR)/$(*F).d; \
	$(CC) $(CFLAGS) -fPIC -MD -MP -MT $@ -MF - $(FLAGS) -c $< -o $@ > $(TMP_DIR)/$(*F).$$$$; \
	sed -e 's#$@:#$@ $(DEP_DIR)/$(*F).d:#' < $(TMP_DIR)/$(*F).$$$$ > $(@:$(OBJ_DIR)/%.o=$(DEP_DIR)/%.d); \
	$(RM) $(TMP_DIR)/$(*F).$$$$
	@printf '$(*F).o created.\n' $(OUTPUT_REDIRECT)

$(LIB_DIR)/$(LIB_NAME): $(OBJS)
	set -e; \
	$(RM) $(DEP_DIR)/$(BIN_NAME).d; \
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -shared -MD -MP -MT $@ -MF - $^ -o $@ > $(TMP_DIR)/$(*F).$$$$; \
	sed 's#$@:#$@ $(DEP_DIR)/$(BIN_NAME).d:#' < $(TMP_DIR)/$(*F).$$$$ > $(DEP_DIR)/$(BIN_NAME).d; \
	$(RM) $(TMP_DIR)/$(*F).$$$$
	@printf '$@ created.\n' $(OUTPUT_REDIRECT)

-include $(DEPS)

