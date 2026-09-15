V=1
SOURCE_DIR=src
BUILD_DIR=build
include $(N64_INST)/include/n64.mk

all: deadbeat_demo.z64

filesystem/deadbeat.sprite: deadbeat.jpg
	@mkdir -p filesystem
	$(N64_MKSPRITE) --format RGBA16 --out filesystem/deadbeat.sprite $<

deadbeat_demo.z64: $(BUILD_DIR)/main.o filesystem/deadbeat.sprite
	$(N64_BINDIR)/mkn64rom -v --title "Skull Demo" --out $@ $< --filesystem filesystem/

clean:
	rm -rf $(BUILD_DIR) filesystem deadbeat_demo.z64

.PHONY: all clean