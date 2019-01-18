#include "AudioDozer.hpp"


struct Potsie : Module {
	enum ParamIds {
		PITCH_PARAM,
		PHASE_PARAM,
		NUM_PARAMS
	};

	enum InputIds {
		PITCH_INPUT,
		PMIX_INPUT,
		NUM_INPUTS
	};

	enum OutputIds {
		SINE_OUTPUT,
		SQUARE_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		BLINK_LIGHT,
		NUM_LIGHTS
	};

	float phase = 0.0;
	float blinkPhase = 0.0;

	Potsie() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};


void Potsie::step() {
	// Implement a simple sine oscillator
	float deltaTime = engineGetSampleTime();

	// Compute the frequency from the pitch parameter and input
	float pitch = params[PITCH_PARAM].value;
	// pitch += inputs[PITCH_INPUT].value;
	pitch = clamp(pitch, -4.0f, 4.0f);
	// The default pitch is C4
	float freq = 261.626f * powf(2.0f, pitch);

	// Get the phase mix
	float phasemix = params[PHASE_PARAM].value;
	
	float mixinput = 0.0f;
	
	if (inputs[PMIX_INPUT].active) {
		mixinput = inputs[PMIX_INPUT].value / 10.0f;
	}

	phasemix = clamp(phasemix + mixinput, 0.0f, 1.0f);

	// Accumulate the phase
	phase += freq * deltaTime;
	if (phase >= 1.0f)
		phase -= 1.0f;

	// Compute the sine output
	float sine = sinf(2.0f * M_PI * phase) + sinf(2.0f * M_PI * phase * phasemix);
	float square = 0.0f;

	if (sine > 0)
		square = 1;
	else
		square = -1;

	outputs[SQUARE_OUTPUT].value = 5.0f * square;
	outputs[SINE_OUTPUT].value = 5.0f * sine / 2.0f;

	// Blink light at 1Hz
	blinkPhase += deltaTime;
	if (blinkPhase >= 1.0f)
		blinkPhase -= 1.0f;

	lights[BLINK_LIGHT].value = (blinkPhase < 0.5f) ? 1.0f : 0.0f;
}


struct PotsieWidget : ModuleWidget {
	PotsieWidget(Potsie *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/Potsie.svg")));

		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(ParamWidget::create<Rogan2PSBlue>(Vec(20, 87), module, Potsie::PITCH_PARAM, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Rogan2PSBlue>(Vec(20, 180), module, Potsie::PHASE_PARAM, 0.0, 1.0, 0.0));

		addInput(Port::create<CL1362Port>(Vec(29, 246), Port::INPUT, module, Potsie::PMIX_INPUT));

		addOutput(Port::create<CL1362Port>(Vec(6, 278), Port::OUTPUT, module, Potsie::SINE_OUTPUT));
		addOutput(Port::create<CL1362Port>(Vec(50, 278), Port::OUTPUT, module, Potsie::SQUARE_OUTPUT));

		addChild(ModuleLightWidget::create<MediumLight<RedLight>>(Vec(41, 59), module, Potsie::BLINK_LIGHT));
	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelPotsie = Model::create<Potsie, PotsieWidget>("AudioDozer", "Potsie", "Potsie is Square", OSCILLATOR_TAG);
