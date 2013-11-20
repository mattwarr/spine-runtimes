
#ifndef VERTEXTRANSLATOR_H
#define VERTEXTRANSLATOR_H

class VertexTranslator {
	public:
		virtual ~VertexTranslator() {};

		/**
		 * Translates a 4x2 quad into a vert buffer corresponding to the translator type.
		 */
		virtual int translate(float* vertices, float* output, int offset, int stride) { return 0; };
};

#endif
