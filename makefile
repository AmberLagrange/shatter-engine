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

# -------- Binary Directories -------- #

BIN_SRC_DIR := $(CURDIR)/src
BIN_INC_DIR := $(CURDIR)/include
BIN_PCH_DIR := $(CURDIR)/pch
BIN_OBJ_DIR := $(CURDIR)/objs
BIN_DEP_DIR := $(CURDIR)/deps
BIN_TMP_DIR := $(CURDIR)/tmp
BIN_OUT_DIR := $(CURDIR)/bin

# -------- Vulkan API Directories -------- #

VK_SRC_DIR := $(CURDIR)/src
VK_INC_DIR := $(CURDIR)/include
VK_PCH_DIR := $(CURDIR)/pch
VK_OBJ_DIR := $(CURDIR)/objs
VK_DEP_DIR := $(CURDIR)/deps
VK_TMP_DIR := $(CURDIR)/tmp
VK_OUT_DIR := $(CURDIR)/api_libraries

# -------- Shader Directories -------- #

SHADER_DIR := $(CURDIR)/shaders
SPV_DIR    := $(CURDIR)/spv

# -------- Binary Name -------- #

BIN_NAME := vulkan

# -------- API Names -------- #

VK_NAME := vulkan_api.so

# -------- Binary Input Files -------- #

BIN_INCS = $(BIN_INC_DIR)/common/core.h

BIN_SRCS = \
	\
	$(BIN_SRC_DIR)/app/app.c \
	\
	$(BIN_SRC_DIR)/logging/logger.c \
	\
	$(BIN_SRC_DIR)/renderer/renderer.c \
	\
	$(BIN_SRC_DIR)/window/glfw.c \
	$(BIN_SRC_DIR)/window/input.c \

# -------- API Shared Library Input Files -------- #

VK_INCS = $(VK_INC_DIR)/common/core.h

VK_SRCS = \
	\
	$(VK_SRC_DIR)/rendering_apis/vulkan/renderer.c \
	\
	$(VK_SRC_DIR)/rendering_apis/vulkan/commands/command_buffer.c \
	$(VK_SRC_DIR)/rendering_apis/vulkan/commands/command_pool.c \
	\
	$(VK_SRC_DIR)/rendering_apis/vulkan/debug/callbacks.c \
	$(VK_SRC_DIR)/rendering_apis/vulkan/debug/debug_utils.c \
	$(VK_SRC_DIR)/rendering_apis/vulkan/debug/validation_layers.c \
	\
	$(VK_SRC_DIR)/rendering_apis/vulkan/devices/extensions.c \
	$(VK_SRC_DIR)/rendering_apis/vulkan/devices/logical.c \
	$(VK_SRC_DIR)/rendering_apis/vulkan/devices/physical.c \
	\
	$(VK_SRC_DIR)/rendering_apis/vulkan/graphics_pipeline/graphics_pipeline.c \
	\
	$(VK_SRC_DIR)/rendering_apis/vulkan/instance/extensions.c \
	$(VK_SRC_DIR)/rendering_apis/vulkan/instance/instance.c \
	\
	$(VK_SRC_DIR)/rendering_apis/vulkan/render_pass/render_pass.c \
	\
	$(VK_SRC_DIR)/rendering_apis/vulkan/shaders/load_shader.c \
	$(VK_SRC_DIR)/rendering_apis/vulkan/shaders/shader_module.c \
	\
	$(VK_SRC_DIR)/rendering_apis/vulkan/swap_chain/frame_buffer.c \
	$(VK_SRC_DIR)/rendering_apis/vulkan/swap_chain/image_view.c \
	$(VK_SRC_DIR)/rendering_apis/vulkan/swap_chain/present_mode.c \
	$(VK_SRC_DIR)/rendering_apis/vulkan/swap_chain/surface_format.c \
	$(VK_SRC_DIR)/rendering_apis/vulkan/swap_chain/swap_chain.c \
	$(VK_SRC_DIR)/rendering_apis/vulkan/swap_chain/swap_chain_support_details.c \
	$(VK_SRC_DIR)/rendering_apis/vulkan/swap_chain/swap_extent.c \
	\
	$(VK_SRC_DIR)/rendering_apis/vulkan/sync/sync_objects.c \
	\
	$(VK_SRC_DIR)/rendering_apis/vulkan/queues/queue_family_indicies.c \
	$(VK_SRC_DIR)/rendering_apis/vulkan/queues/required_queue_families.c \
	\
	$(VK_SRC_DIR)/rendering_apis/vulkan/surfaces/surface.c \

# -------- Binary File Structs -------- #

BIN_PCH_FILE_STRUCT = $(dir $(BIN_INCS:$(BIN_INC_DIR)/%=$(BIN_PCH_DIR)/%))
BIN_OBJ_FILE_STRUCT = $(dir $(BIN_SRCS:$(BIN_SRC_DIR)/%=$(BIN_OBJ_DIR)/%))
BIN_DEP_FILE_STRUCT = $(dir $(BIN_SRCS:$(BIN_SRC_DIR)/%=$(BIN_DEP_DIR)/%))

# -------- API File Structs -------- #

VK_PCH_FILE_STRUCT = $(dir $(VK_INCS:$(VK_INC_DIR)/%=$(VK_PCH_DIR)/%))
VK_OBJ_FILE_STRUCT = $(dir $(VK_SRCS:$(VK_SRC_DIR)/%=$(VK_OBJ_DIR)/%))
VK_DEP_FILE_STRUCT = $(dir $(VK_SRCS:$(VK_SRC_DIR)/%=$(VK_DEP_DIR)/%))

# -------- Binary Output Files --------- #

BIN_PCHS = $(BIN_INCS:$(BIN_INC_DIR)/%=$(BIN_PCH_DIR)/%$(PCH_EXT))
BIN_OBJS = $(BIN_SRCS:$(BIN_SRC_DIR)/%.c=$(BIN_OBJ_DIR)/%.o)

# -------- API Output Files -------- #

VK_PCHS = $(VK_INCS:$(VK_INC_DIR)/%=$(VK_PCH_DIR)/%$(PCH_EXT))
VK_OBJS = $(VK_SRCS:$(VK_SRC_DIR)/%.c=$(VK_OBJ_DIR)/%.o)

# -------- Binary Dependency Files -------- #

BIN_DEPS  = $(BIN_SRCS:$(BIN_SRC_DIR)/%.c=$(BIN_DEP_DIR)/%.d)
BIN_DEPS += $(BIN_DEP_DIR)/$(BIN_NAME).d

# -------- API Dependency Files -------- #

VK_DEPS  = $(VK_SRCS:$(VK_SRC_DIR)/%.c=$(VK_DEP_DIR)/%.d)
VK_DEPS += $(VK_DEP_DIR)/$(VK_NAME).d

# -------- Compiler and Linker flags -------- #

CFLAGS  += -I$(BIN_INC_DIR) -I$(VK_INC_DIR) -I/usr/local/include
LDFLAGS += -L/usr/local/lib
LDLIBS  += -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

# -------- Shader Input Files -------- #

VERT_SRCS = $(SHADER_DIR)/vertex/basic_vertex.vert
FRAG_SRCS = $(SHADER_DIR)/fragment/basic_fragment.frag

# -------- Shader File Structs -------- #

VERT_FILE_STRUCT = $(dir $(VERT_SRCS:$(SHADER_DIR)/%=$(SPV_DIR)/%))
FRAG_FILE_STRUCT = $(dir $(FRAG_SRCS:$(SHADER_DIR)/%=$(SPV_DIR)/%))

# -------- Shader Output Files -------- #

VERT_SPVS = $(VERT_SRCS:$(SHADER_DIR)/%.vert=$(SPV_DIR)/%.spv)
FRAG_SPVS = $(FRAG_SRCS:$(SHADER_DIR)/%.frag=$(SPV_DIR)/%.spv)

SPVS = $(VERT_SPVS) $(FRAG_SPVS)

# -------- Util Targets -------- #

all:
	$(MAKE) dirs
	$(MAKE) bin
	$(MAKE) shaders

dirs:
	# Program
	mkdir -p $(BIN_PCH_DIR)
	mkdir -p $(BIN_PCH_FILE_STRUCT)
	
	mkdir -p $(BIN_OBJ_DIR)
	mkdir -p $(BIN_OBJ_FILE_STRUCT)
	
	mkdir -p $(BIN_DEP_DIR)
	mkdir -p $(BIN_DEP_FILE_STRUCT)
	
	mkdir -p $(BIN_TMP_DIR)
	mkdir -p $(BIN_OUT_DIR)
	
	# APIS
	mkdir -p $(VK_PCH_DIR)
	mkdir -p $(VK_PCH_FILE_STRUCT)
	
	mkdir -p $(VK_OBJ_DIR)
	mkdir -p $(VK_OBJ_FILE_STRUCT)
	
	mkdir -p $(VK_DEP_DIR)
	mkdir -p $(VK_DEP_FILE_STRUCT)
	
	mkdir -p $(VK_TMP_DIR)
	mkdir -p $(VK_OUT_DIR)
	
	# Shaders
	mkdir -p $(SPV_DIR)
	mkdir -p $(VERT_FILE_STRUCT)
	mkdir -p $(FRAG_FILE_STRUCT)

clean:
	# Program
	$(RM) -r $(BIN_PCHS)
	$(RM) -r $(BIN_OBJS)
	$(RM) -r $(BIN_DEPS)
	
	# APIS
	$(RM) -r $(VK_PCHS)
	$(RM) -r $(VK_OBJS)
	$(RM) -r $(VK_DEPS)
	
	# Shaders
	$(RM) -r $(SPVS)

clean_all:
	# Program
	$(RM) -r $(BIN_PCH_DIR)
	$(RM) -r $(BIN_OBJ_DIR)
	$(RM) -r $(BIN_DEP_DIR)
	$(RM) -r $(BIN_TMP_DIR)
	$(RM) -r $(BIN_OUT_DIR)
	
	# APIS	
	$(RM) -r $(VK_PCH_DIR)
	$(RM) -r $(VK_PCH_DIR)
	$(RM) -r $(VK_PCH_DIR)
	$(RM) -r $(VK_PCH_DIR)
	$(RM) -r $(VK_OUT_DIR)
	
	# Shaders
	$(RM) -r $(SPV_DIR)

remake:
	$(MAKE) clean
	$(MAKE) all

remake_all:
	$(MAKE) clean_all
	$(MAKE) all

.PHONY: all dirs clean clean_all remake remake_all bin bin_pchs api api_pchs shaders run

# -------- Program Targets -------- #

bin: dirs bin_pchs shaders $(BIN_OUT_DIR)/$(BIN_NAME)

bin_pchs: $(BIN_PCHS)

$(BIN_PCH_DIR)/%$(PCH_EXT): $(BIN_INC_DIR)/%
	set -e; \
	$(RM) $(BIN_DEP_DIR)/$(*F).d; \
	$(CC) $(CFLAGS) -x c-header -MD -MP -MT $@ -MF - -c $< -o $@ > $(BIN_TMP_DIR)/$(*F).$$$$; \
	sed -e 's#$@:#$@ $(BIN_DEP_DIR)/$(*F).d:#' < $(BIN_TMP_DIR)/$(*F).$$$$ > $(BIN_DEP_DIR)/$(*F).d; \
	$(RM) $(BIN_TMP_DIR)/$(*F).$$$$
	@printf '$(*F)$(PCH_EXT) created.\n' $(OUTPUT_REDIRECT)

$(BIN_OBJ_DIR)/%.o: $(BIN_SRC_DIR)/%.c
	set -e; \
	$(RM) $(BIN_DEP_DIR)/$(*F).d; \
	$(CC) $(CFLAGS) -MD -MP -MT $@ -MF - $(PCH_FLAGS) -c $< -o $@ > $(BIN_TMP_DIR)/$(*F).$$$$; \
	sed -e 's#$@:#$@ $(BIN_DEP_DIR)/$(*F).d:#' < $(BIN_TMP_DIR)/$(*F).$$$$ > $(@:$(BIN_OBJ_DIR)/%.o=$(BIN_DEP_DIR)/%.d); \
	$(RM) $(BIN_TMP_DIR)/$(*F).$$$$
	@printf '$(*F).o created.\n' $(OUTPUT_REDIRECT)

$(BIN_OUT_DIR)/$(BIN_NAME): $(BIN_OBJS) $(VK_OUT_DIR)/$(VK_NAME)
	set -e; \
	$(RM) $(BIN_DEP_DIR)/$(BIN_NAME).d; \
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -MD -MP -MT $@ -MF - $^ -o $@ > $(BIN_TMP_DIR)/$(*F).$$$$; \
	sed 's#$@:#$@ $(BIN_DEP_DIR)/$(BIN_NAME).d:#' < $(BIN_TMP_DIR)/$(*F).$$$$ > $(BIN_DEP_DIR)/$(BIN_NAME).d; \
	$(RM) $(BIN_TMP_DIR)/$(*F).$$$$
	@printf '$@ created.\n' $(OUTPUT_REDIRECT)

-include $(BIN_DEPS)

# -------- API Targets -------- #

vulkan_api: dirs vk_pchs $(VK_OUT_DIR)/$(VK_NAME)

vk_pchs: $(VK_PCHS)

$(VK_PCH_DIR)/%$(PCH_EXT): $(VK_INC_DIR)/%
	set -e; \
	$(RM) $(VK_DEP_DIR)/$(*F).d; \
	$(CC) $(CFLAGS) -fPIC -x c-header -MD -MP -MT $@ -MF - -c $< -o $@ > $(VK_TMP_DIR)/$(*F).$$$$; \
	sed -e 's#$@:#$@ $(VK_DEP_DIR)/$(*F).d:#' < $(VK_TMP_DIR)/$(*F).$$$$ > $(VK_DEP_DIR)/$(*F).d; \
	$(RM) $(VK_TMP_DIR)/$(*F).$$$$
	@printf '$(*F)$(PCH_EXT) created.\n' $(OUTPUT_REDIRECT)

$(VK_OBJ_DIR)/%.o: $(VK_SRC_DIR)/%.c
	set -e; \
	$(RM) $(VK_DEP_DIR)/$(*F).d; \
	$(CC) $(CFLAGS) -fPIC -MD -MP -MT $@ -MF - $(PCH_FLAGS) -c $< -o $@ > $(VK_TMP_DIR)/$(*F).$$$$; \
	sed -e 's#$@:#$@ $(VK_DEP_DIR)/$(*F).d:#' < $(VK_TMP_DIR)/$(*F).$$$$ > $(@:$(VK_OBJ_DIR)/%.o=$(VK_DEP_DIR)/%.d); \
	$(RM) $(VK_TMP_DIR)/$(*F).$$$$
	@printf '$(*F).o created.\n' $(OUTPUT_REDIRECT)

$(VK_OUT_DIR)/$(VK_NAME): $(VK_OBJS)
	set -e; \
	$(RM) $(VK_DEP_DIR)/$(VK_NAME).d; \
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -shared -MD -MP -MT $@ -MF - $^ -o $@ > $(VK_TMP_DIR)/$(*F).$$$$; \
	sed 's#$@:#$@ $(VK_DEP_DIR)/$(VK_NAME).d:#' < $(VK_TMP_DIR)/$(*F).$$$$ > $(VK_DEP_DIR)/$(BIN_NAME).d; \
	$(RM) $(VK_TMP_DIR)/$(*F).$$$$
	@printf '$@ created.\n' $(OUTPUT_REDIRECT)

-include $(VK_DEPS)

# -------- Shader Targets -------- #

$(SPV_DIR)/%.spv: $(SHADER_DIR)/%.vert
	glslc $^ -o $@
	@printf '$(*F).spv created.\n' $(OUTPUT_REDIRECT)

$(SPV_DIR)/%.spv: $(SHADER_DIR)/%.frag
	glslc $^ -o $@
	@printf '$(*F).spv created.\n' $(OUTPUT_REDIRECT)

shaders: $(SPVS)

# -------- Run -------- #

run: $(BIN_OUT_DIR)/$(BIN_NAME)
	$(BIN_OUT_DIR)/$(BIN_NAME) --log-level=info --enable-prefix --enable-color

