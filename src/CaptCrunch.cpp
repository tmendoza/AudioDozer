#include "AudioDozer.hpp"


struct CaptCrunch : Module {
	enum ParamIds {
		DEPTH_PARAM,
		MIX_PARAM,
		NUM_PARAMS
	};		
	
	enum InputIds {
		SIGNAL_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		SIGNAL_OUTPUT,
		NUM_OUTPUTS
	};


	float phase = 0.0;
	float blinkPhase = 0.0;

	CaptCrunch() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};


void CaptCrunch::step() {
	// Implement a simple sine oscillator
	//float deltaTime = engineGetSampleTime();

	// Compute the frequency from the pitch parameter and input
	//float pitch = params[DEPTH_PARAM].value;
	//pitch += inputs[SIGNAL_INPUT].value;
	//pitch = clamp(pitch, -4.0f, 4.0f);
	// The default pitch is C4
	//float freq = 261.626f * powf(2.0f, pitch);

	// Accumulate the phase
	//phase += freq * deltaTime;
	//if (phase >= 1.0f)
	//	phase -= 1.0f;

	// Compute the sine output
	//float sine = sinf(2.0f * M_PI * phase);
	float depthparm = params[DEPTH_PARAM].value;
	float bitdepth = 8.0f;

	if (depthparm >= -3.0 && depthparm <=-1.5)
		bitdepth = 2.0f;
	if (depthparm > -1.5 && depthparm < 0.5)
		bitdepth = 4.0f;
	if (depthparm >= 0.5 && depthparm < 2.0)
		bitdepth = 8.0f;
	if (depthparm >= 2.0 && depthparm <= 3.0)
		bitdepth = 16.0f;

	// Lets grab the input
	float inputval = inputs[SIGNAL_INPUT].value / 5.0f;
	float outval = ceil(bitdepth * inputval) * (1.0f/bitdepth);

	// Write the update info to the output
	outputs[SIGNAL_OUTPUT].value = outval * 5.0f;

}


struct CaptCrunchWidget : ModuleWidget {
	CaptCrunchWidget(CaptCrunch *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/6hp-background-final.svg")));

		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(ParamWidget::create<Rogan2PSBlue>(Vec(22, 87), module, CaptCrunch::DEPTH_PARAM, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Rogan2PSBlue>(Vec(22, 180), module, CaptCrunch::MIX_PARAM, -3.0, 3.0, 0.0));

		addInput(Port::create<CL1362Port>(Vec(6, 272), Port::INPUT, module, CaptCrunch::SIGNAL_INPUT));

		addOutput(Port::create<CL1362Port>(Vec(50, 272), Port::OUTPUT, module, CaptCrunch::SIGNAL_OUTPUT));

	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelCaptCrunch = Model::create<CaptCrunch, CaptCrunchWidget>("AudioDozer", "CaptCrunch", "Capt. Crunch", OSCILLATOR_TAG);
