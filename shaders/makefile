# -------- Printing Utils -------- #

SILENT_FLAG := $(findstring s, $(word 1, $(MAKEFLAGS)))

ifeq ($(SILENT_FLAG), s)
	OUTPUT_REDIRECT = >/dev/null
else
	OUTPUT_REDIRECT =
endif

# -------- Binary Directory -------- #

BIN_DIR ?= $(CURDIR)/../bin

# -------- Shader Directories -------- #

SHADER_IN_DIR := $(CURDIR)
VERT_IN_DIR   := $(SHADER_IN_DIR)/vertex
FRAG_IN_DIR   := $(SHADER_IN_DIR)/fragment

SHADER_OUT_DIR := $(BIN_DIR)/shaders
VERT_OUT_DIR   := $(SHADER_OUT_DIR)/vertex
FRAG_OUT_DIR   := $(SHADER_OUT_DIR)/fragment

SHADER_SPV_DIR := $(SHADER_OUT_DIR)/spv
VERT_SPV_DIR   := $(SHADER_SPV_DIR)/vertex
FRAG_SPV_DIR   := $(SHADER_SPV_DIR)/fragment

# -------- Shader Input Files -------- #

VERT_SRCS = $(VERT_IN_DIR)/basic_vertex.vert
FRAG_SRCS = $(FRAG_IN_DIR)/basic_fragment.frag

# -------- Shader File Structs -------- #

VERT_FILE_STRUCT = $(dir $(VERT_SRCS:$(VERT_IN_DIR)/%=$(VERT_OUT_DIR)/%))
FRAG_FILE_STRUCT = $(dir $(FRAG_SRCS:$(FRAG_IN_DIR)/%=$(FRAG_OUT_DIR)/%))

VERT_SPV_FILE_STRUCT = $(dir $(VERT_SRCS:$(VERT_IN_DIR)/%=$(VERT_SPV_DIR)/%))
FRAG_SPV_FILE_STRUCT = $(dir $(FRAG_SRCS:$(FRAG_IN_DIR)/%=$(FRAG_SPV_DIR)/%))

# -------- Shader Output Files -------- #



VERT_SPVS = $(VERT_SRCS:$(VERT_IN_DIR)/%.vert=$(VERT_SPV_DIR)/%.spv)
FRAG_SPVS = $(FRAG_SRCS:$(FRAG_IN_DIR)/%.frag=$(FRAG_SPV_DIR)/%.spv)
SPVS      = $(VERT_SPVS) $(FRAG_SPVS)

all:
	$(MAKE) dirs
	$(MAKE) shaders

dirs:
	mkdir -p $(SHADER_OUT_DIR)

	mkdir -p $(VERT_FILE_STRUCT)
	mkdir -p $(FRAG_FILE_STRUCT)

	mkdir -p $(SHADER_SPV_DIR)
	mkdir -p $(VERT_SPV_FILE_STRUCT)
	mkdir -p $(FRAG_SPV_FILE_STRUCT)

clean:
	$(RM) -r $(SPVS)

clean_all:
	$(RM) -r $(SHADER_OUT_DIR)

remake:
	$(MAKE) clean
	$(MAKE) all

remake_all:
	$(MAKE) clean_all
	$(MAKE) all

.PHONY: all dirs clean clean_all remake remake_all shaders

# -------- Shader Targets -------- #

$(VERT_SPV_DIR)/%.spv: $(VERT_IN_DIR)/%.vert
	glslc $^ -o $@
	@printf '$(*F).spv created.\n' $(OUTPUT_REDIRECT)

$(FRAG_SPV_DIR)/%.spv: $(FRAG_IN_DIR)/%.frag
	glslc $^ -o $@
	@printf '$(*F).spv created.\n' $(OUTPUT_REDIRECT)

shaders: $(SPVS)

