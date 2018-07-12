
# External defines

# CMSIS_PATH
# CMSIS_DSP_Lib
# CFLAGS
# CPPFLAGS
# ASFLAGS
# CC
# CPP
# AS
# OUTPUT_O_DIR


$(OUTPUT_O_DIR)/%.c.o : $(MathLibraryDir)/%.c
	@echo 	"### Compiling file: $<"
	$(CC) $(CFLAGS) -o $@ $<

$(OUTPUT_O_DIR)/%.cpp.o : $(MathLibraryDir)/%.cpp
	@echo 	"### Compiling file: $<"
	$(CC) $(CPPFLAGS) -o $@ $<

$(OUTPUT_O_DIR)/%.s.o : $(MathLibraryDir)/%.s
	@echo 	"### Compiling file: $<"
	$(CC) $(ASFLAGS) -o $@ $<	