//Output.h

#ifndef __Output_h__
#define __Output_h__
struct Matrix;

class Output
	{

		public:
			Output();
			void ShowScaler(vreal myscaler);
			void ShowMatrix(Matrix &mymatrix);
			void ShowInteger(int myinteger);
			void ShowText (VStr mystr);	

		private:
	};
#endif