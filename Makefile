.PHONY: all list-projects clean

DOCKER_IMAGE ?= aabadie/dotbot:latest
DOCKER_TARGETS ?= all
PACKAGES_DIR_OPT ?=
SEGGER_DIR ?= /opt/segger
BUILD_CONFIG ?= Debug
BUILD_TARGET ?= dotbot-v1
PROJECT_FILE ?= $(BUILD_TARGET).emProject
BOOTLOADER ?= bootloader


PROJECTS ?= $(shell find projects/ -maxdepth 1 -mindepth 1 -type d | tr -d "/" | sed -e s/projects// | sort)

OTAP_APPS ?= $(shell find otap/ -maxdepth 1 -mindepth 1 -type d | tr -d "/" | sed -e s/otap// | sort)
OTAP_APPS := $(filter-out bootloader,$(OTAP_APPS))

SRCS ?= $(shell find bsp/ -name "*.[c|h]") $(shell find drv/ -name "*.[c|h]") $(shell find projects/ -name "*.[c|h]") $(shell find otap/ -name "*.[c|h]")
CLANG_FORMAT ?= clang-format
CLANG_FORMAT_TYPE ?= file

ARTIFACT_ELF = $(foreach app,$(ARTIFACT_PROJECTS),projects/$(app)/Output/$(BUILD_TARGET)/$(BUILD_CONFIG)/Exe/$(app)-$(BUILD_TARGET).elf)
ARTIFACT_HEX = $(ARTIFACT_ELF:.elf=.hex)
ARTIFACTS = $(ARTIFACT_ELF) $(ARTIFACT_HEX)


.PHONY: $(PROJECTS) $(ARTIFACT_PROJECTS) artifacts docker docker-release format check-format

all: $(PROJECTS) $(OTAP_APPS) $(BOOTLOADER)

$(PROJECTS):
	@echo "\e[1mBuilding project $@\e[0m"
	"$(SEGGER_DIR)/bin/emBuild" $(PROJECT_FILE) -project $@ -config $(BUILD_CONFIG) $(PACKAGES_DIR_OPT) -rebuild -verbose
	@echo "\e[1mDone\e[0m\n"

$(OTAP_APPS):
	@echo "\e[1mBuilding otap application $@\e[0m"
	"$(SEGGER_DIR)/bin/emBuild" $(PROJECT_FILE) -project $@ -config $(BUILD_CONFIG) $(PACKAGES_DIR_OPT) -rebuild -verbose
	@echo "\e[1mDone\e[0m\n"

$(BOOTLOADER):
	@echo "\e[1mBuilding bootloader application $@\e[0m"
	"$(SEGGER_DIR)/bin/emBuild" otap/$(BUILD_TARGET)-bootloader.emProject -project $@ -config Release $(PACKAGES_DIR_OPT) -rebuild -verbose
	@echo "\e[1mDone\e[0m\n"

list-projects:
	@echo "\e[1mAvailable projects:\e[0m"
	@echo $(PROJECTS) | tr ' ' '\n'

clean:
	"$(SEGGER_DIR)/bin/emBuild" $(PROJECT_FILE) -config $(BUILD_CONFIG) -clean

distclean: clean
	rm -rf artifacts/

format:
	@$(CLANG_FORMAT) -i --style=$(CLANG_FORMAT_TYPE) $(SRCS)

check-format:
	@$(CLANG_FORMAT) --dry-run --Werror --style=$(CLANG_FORMAT_TYPE) $(SRCS)

artifacts: $(ARTIFACT_PROJECTS)
	@mkdir -p artifacts
	@for artifact in "$(ARTIFACTS)"; do \
		cp $${artifact} artifacts/.; \
		done
	@ls -l artifacts/

docker:
	docker run --rm -i \
		-e BUILD_TARGET="$(BUILD_TARGET)" \
		-e BUILD_CONFIG="$(BUILD_CONFIG)" \
		-e PACKAGES_DIR_OPT="-packagesdir $(SEGGER_DIR)/packages" \
		-e PROJECTS="$(PROJECTS)" \
		-e SEGGER_DIR="$(SEGGER_DIR)" \
		-v $(PWD):/dotbot $(DOCKER_IMAGE) \
		make $(DOCKER_TARGETS)

.PHONY: doc docclean
docclean:
	make -C doc/doxygen clean --no-print-directory
	make -C doc/sphinx clean --no-print-directory
	rm -rf $(addprefix doc/sphinx/_,api examples projects)

doc:
	make -C doc/sphinx linkcheck html --no-print-directory SPHINXOPTS="-W --keep-going -n"
