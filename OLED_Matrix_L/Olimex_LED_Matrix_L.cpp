/*-------------------------------------------------------------------------
  Arduino library for controlling a Olimex 8x8 (non-rgb) matrix. Based on the original library LED_Matrix created by Lutz Reiter, Dec 2014.

  void Clear() - clears the matrix (turns all the matrix lights off)
  void SetPixel(int x, int y) - turns on the light at pixel XY
  void ClearPixel(int x, int y) - turns off the light at pixel XY
  int GetPixel(int x, int y) - returns status of a pixel; 1 if On ; 0 if Off
  void Line (int x1, int y1, int x2, int y2) - lights up a line of pixels from position X1Y1 to position X2Y2
  void DrawRectangle(int x, int y, int w, int h, boolean filled = false) - lights up a rectangle of lights starting from position XY with a width of W, a height of H. If filled is false it lights up only the outline of the rectangle
  void UpdateMatrix() - updates the matrix with the buffer content
  -------------------------------------------------------------------------*/
#include "Olimex_LED_Matrix_L.h"

Olimex_LED_Matrix_L::Olimex_LED_Matrix_L(int latchPin, int dataPin, int clockPin)
{
	this->dataPin = dataPin;
	this->latchPin = latchPin;
	this->clockPin = clockPin;

	pinMode (latchPin, OUTPUT);
	pinMode (clockPin, OUTPUT);
	pinMode (dataPin, OUTPUT);

	this->Clear();
}

void Olimex_LED_Matrix_L::Clear()		// Clears matrix
{
	for (int i=0;i<ROWS;i++)
	this->matrix[i] = 0;
}

// draws a pixel
void Olimex_LED_Matrix_L::SetPixel(int x, int y)
{
	if ((x<0) || (x>=ROWS) || (y<0) || (y>=ROWS))
		return;
	matrix[7-x] |= (1<<y);
}

// erase a Pixel
void Olimex_LED_Matrix_L::ClearPixel(int x, int y)
{
	if ((x<0) || (x>=ROWS) || (y<0) || (y>=ROWS))
		return;

	matrix[7-x] &= ~(1<<y);
}

// Get the value of the coordinates pixel
int Olimex_LED_Matrix_L::GetPixel(int x, int y)
{
	if ((x<0) || (x>=ROWS) || (y<0) || (y>=ROWS))
		return;
	if (matrix[7-x] & (1<<y))
		return 1;
	else
		return 0;
}

void Olimex_LED_Matrix_L::Line (int x1, int y1, int x2, int y2)
{
	int dx, dy, sx, sy, err, e2;
	dx = abs (x2-x1);
	dy = abs (y2-y1);
	if (x1<x2)
		sx = 1;
	else
		sx = -1;
	if (y1<y2)
		sy = 1;
	else
		sy = -1;
	err = dx-dy;
	do
	{
		SetPixel (x1, y1);
		if ((x1 == x2) && (y1 == y2))
			break;
		e2 = 2*err;
		if (e2 > -dy)
		{
			err = err - dy;
			x1 = x1+sx;
		}
		if (e2 < dx)
		{
			err = err + dx;
			y1 = y1 + sy;
		}
	}
	while (1);

	return;
}

// draws a rectangle
void Olimex_LED_Matrix_L::DrawRectangle(int x, int y, int w, int h, boolean filled)
{
	if (filled)
	{
		for(int i=y; i<y+h; i++)
			for(int j=x; j<x+w; j++)
				this->SetPixel(j,i);
	}
	else
	{
		for(int i=y; i<y+h; i++)
		{
			this->SetPixel(x,i);
			this->SetPixel(x+w-1,i);
		}
		for(int i=x; i<x+w; i++)
		{
			this->SetPixel(i,y);
			this->SetPixel(i,y+h-1);
		}
	}
}

void Olimex_LED_Matrix_L::UpdateMatrix()
{
	for (byte i=0; i<ROWS; i++)
	{
		// bitwise operations are faster
		PORTB = PORTB & 0x7F;	//digitalWrite(this->latchPin, LOW);
		shiftOut(this->dataPin, this->clockPin, LSBFIRST, matrix[i]);
		shiftOut(this->dataPin, this->clockPin, LSBFIRST, 1 << i);
    PORTB = PORTB | 0x80; //digitalWrite(this->latchPin, HIGH);
	}

}
