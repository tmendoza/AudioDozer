#include "AudioDozer.hpp"

#include <stdio.h>
#include "SndDefs.h"

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
		TRI_OUTPUT,
		SQUARE_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		BLINK_LIGHT,
		NUM_LIGHTS
	};

	float *sinoutbuf = NULL;
	float *trioutbuf = NULL;
	float *sqroutbuf = NULL;
	float *sawoutbuf = NULL;

	float *sinwavet = NULL;
	float *triwavet = NULL;
	float *sqrwavet = NULL;
	float *sawwavet = NULL;

	float triidx = 0.0f;
	float sqridx = 0.0f;
	float sawidx = 0.0f; 
	float sinidx = 0.0f;

	float phase = 0.0f;
	float blinkPhase = 0.0f;

	//GenWave *sw = NULL;
	//GenWave *tw = NULL;

	Potsie() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
		info("Init output buffer...");
		sinoutbuf = (float *) malloc(def_vsize);
		trioutbuf = (float *) malloc(def_vsize);
		sqroutbuf = (float *) malloc(def_vsize);
		sawoutbuf = (float *) malloc(def_vsize);

		info("init sine wave lookup table...");
		sinwavet = sinus_table();
		triwavet = triang_table(11);
		sqrwavet = sqr_table(11);
		sawwavet = saw_table(11);;
	}

	void step() override;
	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};

void Potsie::step() {
	// Implement a simple sine oscillator
	float deltaTime = engineGetSampleTime();
	
	float basefreq = 130.81f;

	// Here in case we add a switch to alter the base frequency
	//genwave_setfreq(sw, basefreq);
	//genwave_setfreq(tw, basefreq);

	// Compute the frequency from the pitch parameter and input
	float pitch = params[PITCH_PARAM].value;
	pitch += inputs[PITCH_INPUT].value;
	pitch = clamp(pitch, -4.0f, 4.0f);

	osci(sinoutbuf, 5.0f, (basefreq * powf(2.0f, pitch)), sinwavet, &sinidx);
	osci(trioutbuf, 5.0f, (basefreq * powf(2.0f, pitch)), triwavet, &triidx);
	osci(sqroutbuf, 5.0f, (basefreq * powf(2.0f, pitch)), sqrwavet, &sqridx);
	osci(sawoutbuf, 5.0f, (basefreq * powf(2.0f, pitch)), sawwavet, &sawidx);
	// The default pitch is C4
	//FrqValue newSfreq = sw->frq * powf(2.0f, pitch);
	//FrqValue newTfreq = tw->frq * powf(2.0f, pitch);
	// Get the phase mix
	//float phasemix = params[PHASE_PARAM].value;
	
	//float mixinput = 0.0f;
	
	//if (inputs[PMIX_INPUT].active) {
	//	mixinput = inputs[PMIX_INPUT].value / 10.0f;
	//}

	//phasemix = clamp(phasemix + mixinput, 0.0f, 1.0f);

	// Accumulate the phase
	//phase += freq * deltaTime;
	//if (phase >= 1.0f)
	//	phase -= 1.0f;
	//genwave_sin_modulate(sw, newSfreq);
	//genwave_tri_modulate(tw, newTfreq);

	// Compute the sine output
	//float sine = sinf(2.0f * M_PI * phase); // + sinf(2.0f * M_PI * phase * phasemix);


	outputs[TRI_OUTPUT].value = *trioutbuf;
	outputs[SINE_OUTPUT].value = *sinoutbuf;
	outputs[SQUARE_OUTPUT].value = (*sqroutbuf + *sawoutbuf) / 2.0f;

	///float square = 0.0f;

	//if (v > 0)
	//	square = 5.0f;
	
	//if (v < 0)
	//	square = -5.0f;

	//outputs[SQUARE_OUTPUT].value = square;

	// Blink light at 1Hz
	//blinkPhase += deltaTime;
	//if (blinkPhase >= 1.0f)
	//	blinkPhase -= 1.0f;

	//lights[BLINK_LIGHT].value = (blinkPhase < 0.5f) ? 1.0f : 0.0f;
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

		addOutput(Port::create<CL1362Port>(Vec(29, 246), Port::OUTPUT, module, Potsie::TRI_OUTPUT));
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
