# -------- Printing Utils -------- #

SILENT_FLAG := $(findstring s, $(word 1, $(MAKEFLAGS)))

ifeq ($(SILENT_FLAG), s)
	OUTPUT_REDIRECT = >/dev/null
else
	OUTPUT_REDIRECT =
endif

# -------- Shader Directories -------- #

SHADER_DIR := $(CURDIR)
SPV_DIR    := $(CURDIR)/../spv

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

all:
	$(MAKE) dirs
	$(MAKE) shaders

dirs:
	mkdir -p $(SPV_DIR)
	mkdir -p $(VERT_FILE_STRUCT)
	mkdir -p $(FRAG_FILE_STRUCT)

clean:
	$(RM) -r $(SPVS)

clean_all:
	$(RM) -r $(SPV_DIR)

remake:
	$(MAKE) clean
	$(MAKE) all

remake_all:
	$(MAKE) clean_all
	$(MAKE) all

.PHONY: all dirs clean clean_all remake remake_all shaders

# -------- Shader Targets -------- #

$(SPV_DIR)/%.spv: $(SHADER_DIR)/%.vert
	glslc $^ -o $@
	@printf '$(*F).spv created.\n' $(OUTPUT_REDIRECT)

$(SPV_DIR)/%.spv: $(SHADER_DIR)/%.frag
	glslc $^ -o $@
	@printf '$(*F).spv created.\n' $(OUTPUT_REDIRECT)

shaders: $(SPVS)

