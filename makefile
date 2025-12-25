SRC_DIR := $(CURDIR)/src
INC_DIR := $(CURDIR)/include
PCH_DIR := $(CURDIR)/pch
OBJ_DIR := $(CURDIR)/objs
DEP_DIR := $(CURDIR)/deps
TMP_DIR := $(CURDIR)/tmp
BIN_DIR := $(CURDIR)/bin

BIN_NAME := vulkan

SILENT_FLAG := $(findstring s, $(word 1, $(MAKEFLAGS)))

ifeq ($(SILENT_FLAG), s)
	OUTPUT_REDIRECT = >/dev/null
else
	OUTPUT_REDIRECT =
endif

ifeq ($(CC), clang)
	PCH_EXT    = .pch
	PCH_FLAGS := $(foreach PCH, $(PCHS), -include-pch $(PCH))
else
	PCH_EXT    = .gch
	PCH_FLAGS := $(foreach PCH, $(PCHS), -include $(PCH))
endif

# For precompiled headers
INCS  = $(INC_DIR)/common/core.h

SRCS = $(SRC_DIR)/app/app.c \
	   \
	   $(SRC_DIR)/logging/logger.c \
	   \
	   $(SRC_DIR)/renderers/glfw.c \
	   $(SRC_DIR)/renderers/renderer.c \
	   \
	   $(SRC_DIR)/renderers/vulkan/callbacks.c \
	   $(SRC_DIR)/renderers/vulkan/debug_utils.c \
	   $(SRC_DIR)/renderers/vulkan/extensions.c \
	   $(SRC_DIR)/renderers/vulkan/instance.c \
	   $(SRC_DIR)/renderers/vulkan/renderer.c \
	   $(SRC_DIR)/renderers/vulkan/validation_layers.c \
	   \
	   $(SRC_DIR)/renderers/vulkan/devices/extensions.c \
	   $(SRC_DIR)/renderers/vulkan/devices/logical.c \
	   \
	   $(SRC_DIR)/renderers/vulkan/devices/physical.c \
	   \
	   $(SRC_DIR)/renderers/vulkan/image_view/image_view.c \
	   \
	   $(SRC_DIR)/renderers/vulkan/swap_chain/present_mode.c \
	   $(SRC_DIR)/renderers/vulkan/swap_chain/surface_format.c \
	   $(SRC_DIR)/renderers/vulkan/swap_chain/swap_chain.c \
	   $(SRC_DIR)/renderers/vulkan/swap_chain/swap_chain_support_details.c \
	   $(SRC_DIR)/renderers/vulkan/swap_chain/swap_extent.c \
	   \
	   $(SRC_DIR)/renderers/vulkan/queues/queue_family_indicies.c \
	   $(SRC_DIR)/renderers/vulkan/queues/required_queue_families.c \
	   \
	   $(SRC_DIR)/renderers/vulkan/surfaces/surface.c \

PCH_FILE_STRUCT = $(dir $(INCS:$(INC_DIR)/%=$(PCH_DIR)/%))
OBJ_FILE_STRUCT = $(dir $(SRCS:$(SRC_DIR)/%=$(OBJ_DIR)/%))
DEP_FILE_STRUCT = $(dir $(SRCS:$(SRC_DIR)/%=$(DEP_DIR)/%))

PCHS  = $(INCS:$(INC_DIR)/%=$(PCH_DIR)/%$(PCH_EXT))
OBJS  = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS  = $(SRCS:$(SRC_DIR)/%.c=$(DEP_DIR)/%.d)
DEPS += $(DEP_DIR)/$(BIN_NAME).d

CFLAGS  += -I$(INC_DIR) -I/usr/local/include -Icglm/include
LDFLAGS += -L/usr/local/lib
LDLIBS  += -lcglm -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

bin: dirs pchs $(BIN_DIR)/$(BIN_NAME)

run: $(BIN_DIR)/$(BIN_NAME)
	$(BIN_DIR)/$(BIN_NAME) --verbose

dirs:
	mkdir -p $(PCH_DIR)
	mkdir -p $(PCH_FILE_STRUCT)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_FILE_STRUCT)
	mkdir -p $(DEP_DIR)
	mkdir -p $(DEP_FILE_STRUCT)
	mkdir -p $(TMP_DIR)
	mkdir -p $(BIN_DIR)

pchs: $(PCHS)

remake:
	$(MAKE) clean
	$(MAKE) bin

clean:
	$(RM) -r $(PCH_DIR)
	$(RM) -r $(OBJ_DIR)
	$(RM) -r $(DEP_DIR)
	$(RM) -r $(TMP_DIR)
	$(RM) -r $(BIN_DIR)

$(PCH_DIR)/%$(PCH_EXT): $(INC_DIR)/%
	set -e; \
	$(RM) $(DEP_DIR)/$(*F).d; \
	$(CC) $(CFLAGS) -x c-header -MD -MP -MT $@ -MF - -c $< -o $@ > $(TMP_DIR)/$(*F).$$$$; \
	sed -e 's#$@:#$@ $(DEP_DIR)/$(*F).d:#' < $(TMP_DIR)/$(*F).$$$$ > $(DEP_DIR)/$(*F).d; \
	$(RM) $(TMP_DIR)/$(*F).$$$$
	@printf '$(*F)$(PCH_EXT) created.\n' $(OUTPUT_REDIRECT)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	set -e; \
	$(RM) $(DEP_DIR)/$(*F).d; \
	$(CC) $(CFLAGS) -MD -MP -MT $@ -MF - $(PCH_FLAGS) -c $< -o $@ > $(TMP_DIR)/$(*F).$$$$; \
	sed -e 's#$@:#$@ $(DEP_DIR)/$(*F).d:#' < $(TMP_DIR)/$(*F).$$$$ > $(@:$(OBJ_DIR)/%.o=$(DEP_DIR)/%.d); \
	$(RM) $(TMP_DIR)/$(*F).$$$$
	@printf '$(*F).o created.\n' $(OUTPUT_REDIRECT)

$(BIN_DIR)/$(BIN_NAME): $(OBJS)
	set -e; \
	$(RM) $(DEP_DIR)/$(BIN_NAME).d; \
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -MD -MP -MT $@ -MF - $^ -o $@ > $(TMP_DIR)/$(*F).$$$$; \
	sed 's#$@:#$@ $(DEP_DIR)/$(BIN_NAME).d:#' < $(TMP_DIR)/$(*F).$$$$ > $(DEP_DIR)/$(BIN_NAME).d; \
	$(RM) $(TMP_DIR)/$(*F).$$$$
	@printf '$(BIN_NAME) created.\n' $(OUTPUT_REDIRECT)

.PHONY: bin run dirs pchs clean remakee

-include $(DEPS)

