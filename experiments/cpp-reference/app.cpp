/*
 * app.cpp
 *
 *  Created on: Jun 12, 2024
 *      Author: monica
 */

#include "app.h"
#include "FreeRTOS.h"
#include "matrix.h"

int k = 1;

static float Adata[2*2] = {0.9324, 0, 0, 0.9324};
static float Bdata[2*3] = {0.6439, -0.3219, -0.3219, 0, 0.5576, -0.5576};


static float xkdata[2*1];
static float ukdata[1*3];

static float xk1data[2*1];
static float uk1data[1*3];

static float xrefdata[2*1] = {5, 5};
static Matrix_t A = {2, 2, Adata};
static Matrix_t B = {2, 3, Bdata};


static Matrix_t xk = {2, 1, xkdata};
static Matrix_t uk = {1, 3, ukdata};

static Matrix_t uk1 = {1, 3, uk1data};
static Matrix_t x_ref = {2, 1, xrefdata};


constexpr uint8_t NumberOfStates = 7;
constexpr float switch_state[NumberOfStates][3] = {
	{0, 0, 0},  //0
	{0, 0, 1},  //1
	{0, 1, 0},  //2
	{0, 1, 1},  //3
	{1, 0, 0},  //4
	{1, 1, 0},  //5
	{1, 0, 1}   //6
};

unsigned int selectBestCombination();

void main_cpp()
{

    // Parameters for the loop
    int k_test = 50;

    // equation
    /*
    Phase A
    y = 11.2 x - 18.094

    Phase B
    y = 11.795 x - 19.049
    */
    for ( k = 1; k < k_test; ++k)
    {

		memcpy(uk.data, &matrixElement(switch_state, selectBestCombination(), 0), 3);

		matrixMultiply(A, xk, xk);           // xk = A * xk + B * uk;
		matrixMultiplyAddingToResult(B, uk, xk);  // x_x = x_p + B * uk1
	}

	while (1)
	{

	}

}


unsigned int selectBestCombination() {
      double guk;
      double lambda = 0.01; // switch
      double lambda_i = 10.0; // current


      float g_idata[2*1]= {0,0};
      static Matrix_t g_i = {2, 1, g_idata};

      float g_u[1*3]= {0,0,0};

      float x_p[2*1]= {0,0};


      double min_g = 1000000.0;
      int min_index = 0;

      for (int combination = 0; combination < NumberOfStates; ++combination)
      {

				memcpy(uk1.data, &matrixElement(switch_state, combination, 0), 3);

				  // x_p << A * xk + B * uk1
				matrixMultiply(A, xk, xp);           // x_p = A * xk
				matrixMultiplyAddingToResult(B, uk1, xp);  // x_x = x_p + B * uk1

				g_i =  (x_ref - x_p);
				float g_i_norm = sqrt(g_i^2 + g_i^2);

				if (k==1)
				{

					(uk1.data, &matrixElement(switch_state, combination, 0), 3)
					  g_u = ( (uk).transpose() - uk1);
				}
				else
				{
					  g_u = ( (uk) - uk1 );
				}

				float g_u_norm = sqrt(g_u^2 + g_u^2);
				guk = g_u_norm * lambda + g_i_norm * lambda_i;

				if (guk < min_g )
				{
					  min_g = guk ;
					  min_index = combination ;
				}
      }

      return min_index;
}
