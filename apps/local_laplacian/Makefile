include ../support/Makefile.inc

BIN ?= bin

all: $(BIN)/process

$(BIN)/local_laplacian.generator: local_laplacian_generator.cpp $(GENERATOR_DEPS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -g -fno-rtti $(filter-out %.h,$^) -o $@ $(LDFLAGS) $(HALIDE_SYSTEM_LDFLAGS)

$(BIN)/local_laplacian.a: $(BIN)/local_laplacian.generator
	@mkdir -p $(@D)
	$^ -g local_laplacian -o $(BIN) -f local_laplacian target=$(HL_TARGET) auto_schedule=false

$(BIN)/local_laplacian_auto_schedule.a: $(BIN)/local_laplacian.generator
	@mkdir -p $(@D)
	$^ -g local_laplacian -o $(BIN) -f local_laplacian_auto_schedule target=$(HL_TARGET)-no_runtime auto_schedule=true

$(BIN)/process: process.cpp $(BIN)/local_laplacian.a $(BIN)/local_laplacian_auto_schedule.a
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I$(BIN) -Wall -O3 $^ -o $@ $(LDFLAGS) $(IMAGE_IO_FLAGS) $(CUDA_LDFLAGS) $(OPENCL_LDFLAGS) $(OPENGL_LDFLAGS)

$(BIN)/out.png: $(BIN)/process
	@mkdir -p $(@D)
	$(BIN)/process $(IMAGES)/rgb.png 8 1 1 10 $(BIN)/out.png

# Build rules for generating a visualization of the pipeline using HalideTraceViz
$(BIN)/viz/local_laplacian.a: $(BIN)/local_laplacian.generator
	@mkdir -p $(@D)
	$^ -g local_laplacian -o $(BIN)/viz target=$(HL_TARGET)-trace_loads-trace_stores-trace_realizations pyramid_levels=6

$(BIN)/process_viz: process.cpp $(BIN)/viz/local_laplacian.a
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -DNO_AUTO_SCHEDULE -I$(BIN)/viz -Wall -O3 $^ -o $@ $(LDFLAGS) $(IMAGE_IO_FLAGS) $(CUDA_LDFLAGS) $(OPENCL_LDFLAGS) $(OPENGL_LDFLAGS)

../../bin/HalideTraceViz:
	$(MAKE) -C ../../ bin/HalideTraceViz

$(BIN)/local_laplacian.mp4: $(BIN)/process_viz ../../bin/HalideTraceViz
	@mkdir -p $(@D)
	bash viz.sh

clean:
	rm -rf $(BIN)
