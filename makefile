# -------- Printing Utils -------- #

SILENT_FLAG := $(findstring s, $(word 1, $(MAKEFLAGS)))

ifeq ($(SILENT_FLAG), s)
	OUTPUT_REDIRECT = >/dev/null
else
	OUTPUT_REDIRECT =
endif

# -------- PCH Requirements -------- #

ifeq ($(CC), clang)
	PCH_EXT    = .pch
	PCH_FLAGS := $(foreach PCH, $(PCHS), -include-pch $(PCH))
else
	PCH_EXT    = .gch
	PCH_FLAGS := $(foreach PCH, $(PCHS), -include $(PCH))
endif

# -------- Directories -------- #

SRC_DIR := $(CURDIR)/src
INC_DIR := $(CURDIR)/include
OBJ_DIR := $(CURDIR)/objs
DEP_DIR := $(CURDIR)/deps
TMP_DIR := $(CURDIR)/tmp
BIN_DIR := $(CURDIR)/bin

# -------- Shader Directories -------- #

SHADER_DIR := $(CURDIR)/shaders

# -------- Binary Name -------- #

BIN_NAME := shatter_engine

# -------- Binary Input Files -------- #

INCS = $(INC_DIR)/common/core.h

SRCS = $(SRC_DIR)/app/app.c \
	   \
	   $(SRC_DIR)/dynamic_loader/dynamic_loader.c \
	   \
	   $(SRC_DIR)/renderer/renderer.c \
	   \
	   $(SRC_DIR)/window/glfw.c \
	   $(SRC_DIR)/window/input.c \
	   $(SRC_DIR)/window/window.c \

# -------- Binary File Structs -------- #

OBJ_FILE_STRUCT = $(dir $(SRCS:$(SRC_DIR)/%=$(OBJ_DIR)/%))
DEP_FILE_STRUCT = $(dir $(SRCS:$(SRC_DIR)/%=$(DEP_DIR)/%))

# -------- Binary Output Files --------- #

PCHS = $(INCS:$(INC_DIR)/%=$(INC_DIR)/%$(PCH_EXT))
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# -------- Binary Dependency Files -------- #

DEPS  = $(SRCS:$(SRC_DIR)/%.c=$(DEP_DIR)/%.d)
DEPS += $(DEP_DIR)/$(BIN_NAME).d

# -------- API Info -------- #

API_DIR = $(CURDIR)/graphics_apis
APIS    = $(API_DIR)/vulkan $(API_DIR)/opengl

API_LIB_DIR = $(BIN_DIR)/api_libraries

# -------- Logger Info -------- #

LOGGER_DIR := $(CURDIR)/logger
LOGGER_LIB := $(LOGGER_DIR)/lib/logger.so

# -------- Compiler and Linker flags -------- #

CFLAGS  += -I$(INC_DIR) -I$(LOGGER_DIR)/include
LDLIBS  += -lglfw

# -------- Util Targets -------- #

all:
	$(MAKE) dirs
	
	$(MAKE) -C $(LOGGER_DIR)
	
	$(MAKE) bin
	
	$(MAKE) apis
	
	$(MAKE) shaders

dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_FILE_STRUCT)
	
	mkdir -p $(DEP_DIR)
	mkdir -p $(DEP_FILE_STRUCT)
	
	mkdir -p $(TMP_DIR)
	mkdir -p $(BIN_DIR)
	
	$(MAKE) -C $(LOGGER_DIR) dirs
	
	printf '$(APIS)' | xargs -d' ' -I{} make API_LIB_DIR=$(API_LIB_DIR) -C {} dirs
	
	$(MAKE) -C $(SHADER_DIR) dirs

clean:
	$(RM) $(PCHS)
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(BIN_DIR)/$(BIN_NAME)
	
	$(MAKE) -C $(LOGGER_DIR) clean
	
	printf '$(APIS)' | xargs -d' ' -I{} make API_LIB_DIR=$(API_LIB_DIR) -C {} clean
	
	$(MAKE) -C $(SHADER_DIR) clean

clean_all:
	$(RM) -r $(OBJ_DIR)
	$(RM) -r $(DEP_DIR)
	$(RM) -r $(TMP_DIR)
	$(RM) -r $(BIN_DIR)
	$(RM)    $(PCHS)
	
	$(MAKE) -C $(LOGGER_DIR) clean_all
	
	printf '$(APIS)' | xargs -d' ' -I{} make API_LIB_DIR=$(API_LIB_DIR) -C {} clean_all
	
	$(MAKE) -C $(SHADER_DIR) clean_all

remake:
	$(MAKE) clean
	$(MAKE) all

remake_all:
	$(MAKE) clean_all
	$(MAKE) all

.PHONY: all dirs clean clean_all remake remake_all bin pchs apis shaders run

# -------- Program Targets -------- #

bin: pchs $(BIN_DIR)/$(BIN_NAME)

pchs: $(PCHS)

$(INC_DIR)/%$(PCH_EXT): $(INC_DIR)/%
	set -e; \
	$(RM) $(DEP_DIR)/$(*F).d; \
	$(CC) $(CFLAGS) -x c-header -MD -MP -MT $@ -MF - -c $< -o $@ > $(TMP_DIR)/$(*F).$$$$; \
	sed -e 's#$@:#$@ $(DEP_DIR)/$(*F).d:#' < $(TMP_DIR)/$(*F).$$$$ > $(DEP_DIR)/$(*F).d; \
	$(RM) $(TMP_DIR)/$(*F).$$$$
	@printf '$(*F)$(PCH_EXT) created.\n' $(OUTPUT_REDIRECT)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	set -e; \
	$(RM) $(DEP_DIR)/$(*F).d; \
	$(CC) $(CFLAGS) -MD -MP -MT $@ -MF - $(FLAGS) -c $< -o $@ > $(TMP_DIR)/$(*F).$$$$; \
	sed -e 's#$@:#$@ $(DEP_DIR)/$(*F).d:#' < $(TMP_DIR)/$(*F).$$$$ > $(@:$(OBJ_DIR)/%.o=$(DEP_DIR)/%.d); \
	$(RM) $(TMP_DIR)/$(*F).$$$$
	@printf '$(*F).o created.\n' $(OUTPUT_REDIRECT)

$(BIN_DIR)/$(BIN_NAME): $(LOGGER_LIB) $(OBJS)
	set -e; \
	$(RM) $(DEP_DIR)/$(BIN_NAME).d; \
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -MD -MP -MT $@ -MF - $^ -o $@ > $(TMP_DIR)/$(*F).$$$$; \
	sed 's#$@:#$@ $(DEP_DIR)/$(BIN_NAME).d:#' < $(TMP_DIR)/$(*F).$$$$ > $(DEP_DIR)/$(BIN_NAME).d; \
	$(RM) $(TMP_DIR)/$(*F).$$$$
	@printf '$@ created.\n' $(OUTPUT_REDIRECT)

-include $(DEPS)

# -------- API Targets -------- #

apis:
	printf '$(APIS)' | xargs -d' ' -I{} make API_LIB_DIR=$(API_LIB_DIR) \
											 ENGINE_INC=$(INC_DIR) \
											 LOGGER_INC=$(LOGGER_DIR)/include \
											 -C {}

# -------- Logger Targets -------- #

$(LOGGER_LIB):
	$(MAKE) -C $(LOGGER_DIR)

# -------- Shader Targets -------- #

shaders:
	$(MAKE) BIN_DIR=$(BIN_DIR) API_DIR=$(BIN_DIR)/api_libraries -C $(SHADER_DIR)

# -------- Run -------- #

run: $(BIN_DIR)/$(BIN_NAME) apis shaders
	$(BIN_DIR)/$(BIN_NAME) --log-level=info --enable-log-prefix-colors --enable-log-message-colors

