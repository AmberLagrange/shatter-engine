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

API_LIB_DIR ?= $(CURDIR)/../../bin/api_libraries
ENGINE_INC  ?= $(CURDIR)/../../include
LOGGER_INC  ?= $(CURDIR)/../../logger/include

# -------- API Name -------- #

API_NAME := opengl_api.so

# -------- Input Files -------- #

SRCS = $(SRC_DIR)/glad.c \
	   $(SRC_DIR)/opengl_renderer.c \
	   $(SRC_DIR)/vtable.c

# -------- File Structs -------- #

OBJ_FILE_STRUCT = $(dir $(SRCS:$(SRC_DIR)/%=$(OBJ_DIR)/%))
DEP_FILE_STRUCT = $(dir $(SRCS:$(SRC_DIR)/%=$(DEP_DIR)/%))

# -------- Output Files -------- #

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# -------- Dependency Files -------- #

DEPS  = $(SRCS:$(SRC_DIR)/%.c=$(DEP_DIR)/%.d)
DEPS += $(DEP_DIR)/$(API_NAME).d

# -------- Compiler and Linker flags -------- #

CFLAGS += -I$(INC_DIR) -I$(ENGINE_INC) -I$(LOGGER_INC)
LDLIBS += -lGL

# -------- Util Targets -------- #

all: dirs pchs $(API_LIB_DIR)/$(API_NAME)

dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_FILE_STRUCT)
	
	mkdir -p $(DEP_DIR)
	mkdir -p $(DEP_FILE_STRUCT)
	
	mkdir -p $(TMP_DIR)
	mkdir -p $(API_LIB_DIR)

clean:
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(API_LIB_DIR)/$(API_NAME)

clean_all:
	$(RM) -r $(OBJ_DIR)
	$(RM) -r $(DEP_DIR)
	$(RM) -r $(TMP_DIR)
	$(RM) -r $(API_LIB_DIR)

remake:
	$(MAKE) clean
	$(MAKE) all

remake_all:
	$(MAKE) clean_all
	$(MAKE) all

.PHONY: all dirs pchs clean clean_all remake remake_all

# -------- API Targets -------- #

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	set -e; \
	$(RM) $(DEP_DIR)/$(*F).d; \
	$(CC) $(CFLAGS) -fPIC -MD -MP -MT $@ -MF - $(PCH_FLAGS) -c $< -o $@ > $(TMP_DIR)/$(*F).$$$$; \
	sed -e 's#$@:#$@ $(DEP_DIR)/$(*F).d:#' < $(TMP_DIR)/$(*F).$$$$ > $(@:$(OBJ_DIR)/%.o=$(DEP_DIR)/%.d); \
	$(RM) $(TMP_DIR)/$(*F).$$$$
	@printf '$(*F).o created.\n' $(OUTPUT_REDIRECT)

$(API_LIB_DIR)/$(API_NAME): $(OBJS)
	set -e; \
	$(RM) $(DEP_DIR)/$(API_NAME).d; \
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -shared -MD -MP -MT $@ -MF - $^ -o $@ > $(TMP_DIR)/$(*F).$$$$; \
	sed 's#$@:#$@ $(DEP_DIR)/$(NAME).d:#' < $(TMP_DIR)/$(*F).$$$$ > $(DEP_DIR)/$(BIN_NAME).d; \
	$(RM) $(TMP_DIR)/$(*F).$$$$
	@printf '$@ created.\n' $(OUTPUT_REDIRECT)

-include $(DEPS)

