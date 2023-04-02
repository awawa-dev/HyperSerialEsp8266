/* base.h
*
*  MIT License
*
*  Copyright (c) 2023 awawa-dev
*
*  https://github.com/awawa-dev/HyperSerialEsp8266
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  The above copyright notice and this permission notice shall be included in all
*  copies or substantial portions of the Software.

*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*  SOFTWARE.
 */

#ifndef BASE_H
#define BASE_H

class Base
{
	// LED strip number
	int ledsNumber = 0;
	// NeoPixelBusLibrary primary object
	LED_DRIVER* ledStrip1 = nullptr;
	// frame is set and ready to render
	bool readyToRender = false;

	public:
		// static data buffer for the loop
		uint8_t buffer[MAX_BUFFER];
		// current queue position
		uint16_t queueCurrent = 0;
		// queue end position
		volatile uint16_t queueEnd = 0;

		inline int getLedsNumber()
		{
			return ledsNumber;
		}

		inline LED_DRIVER* getLedStrip1()
		{
			return ledStrip1;
		}

		void initLedStrip(int count)
		{
			if (ledStrip1 != nullptr)
			{
				delete ledStrip1;
				ledStrip1 = nullptr;
			}

			ledsNumber = count;

			if (ledStrip1 == nullptr)
			{
				ledStrip1 = new LED_DRIVER(ledsNumber);
				ledStrip1->Begin();
			}
		}

		/**
		 * @brief Check if there is already prepared frame to display
		 *
		 * @return true
		 * @return false
		 */
		inline bool hasLateFrameToRender()
		{
			return readyToRender;
		}

		inline void renderLeds(bool newFrame)
		{
			if (newFrame)
				readyToRender = true;

			if (readyToRender &&
				(ledStrip1 != nullptr && ledStrip1->CanShow()))
			{
				statistics.increaseShow();
				readyToRender = false;

				// display segments
				ledStrip1->Show(false);
			}
		}

		inline bool setStripPixel(uint16_t pix, ColorDefinition &inputColor)
		{
			if (pix < ledsNumber)
			{
				ledStrip1->SetPixelColor(pix, inputColor);
			}

			return (pix + 1 < ledsNumber);
		}
} base;

#endif