M = matrix([[1.000,0.000,0.065,0.003,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.032,0.000,0.000,0.039,0.000,0.015,0.032,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.184,0.000,0.020,0.000,0.008,0.018,0.000,0.000,0.000,0.003,0.000,0.066,0.006,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.033,0.010,0.000],
[0.000,1.000,0.000,0.000,0.000,0.038,0.006,0.000,0.000,0.053,0.000,0.000,0.038,0.000,0.032,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.044,0.000,0.006,0.000,0.000,0.000,0.000,0.000,0.000,0.032,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.035,0.000,0.012,0.000,0.000,0.000,0.000,0.000,0.000,0.035,0.000],
[0.027,0.000,1.000,0.001,0.000,0.007,0.014,0.104,0.000,0.000,0.000,0.023,0.032,0.000,0.000,0.000,0.007,0.045,0.035,0.000,0.000,0.032,0.007,0.000,0.000,0.011,0.000,0.000,0.029,0.000,0.006,0.044,0.000,0.016,0.000,0.024,0.000,0.023,0.011,0.001,0.002,0.023,0.000,0.011,0.011,0.007,0.000,0.034,0.028,0.000,0.014,0.000,0.000,0.065,0.089,0.000],
[0.005,0.000,0.006,1.000,0.000,0.000,0.000,0.023,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.002,0.000,0.000,0.000,0.000,0.000,0.000,0.076,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.116,0.000,0.000,0.000,0.038,0.056,0.000,0.056,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.002,0.000],
[0.000,0.000,0.000,0.000,1.000,0.014,0.013,0.000,0.099,0.000,0.000,0.000,0.000,0.000,0.015,0.000,0.000,0.000,0.000,0.000,0.186,0.000,0.000,0.000,0.000,0.015,0.000,0.011,0.000,0.018,0.000,0.000,0.000,0.000,0.000,0.000,0.033,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.014,0.000,0.014,0.000,0.000,0.000,0.000,0.000,0.000,0.043,0.000],
[0.000,0.083,0.051,0.000,0.083,1.000,0.031,0.021,0.000,0.083,0.000,0.000,0.026,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.062,0.000,0.083,0.000,0.000,0.007,0.000,0.083,0.083,0.012,0.000,0.095,0.000,0.000,0.000,0.026,0.000,0.000,0.000,0.000,0.000,0.026,0.000,0.000,0.000,0.179,0.000,0.022,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000],
[0.000,0.005,0.048,0.000,0.033,0.014,1.000,0.035,0.000,0.031,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.019,0.004,0.000,0.004,0.000,0.015,0.000,0.000,0.000,0.000,0.044,0.014,0.088,0.000,0.114,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.027,0.000,0.000,0.000,0.015,0.000,0.039,0.027,0.000,0.000,0.000,0.000,0.037,0.007,0.000],
[0.000,0.000,0.062,0.003,0.000,0.002,0.006,1.000,0.000,0.000,0.000,0.020,0.091,0.000,0.000,0.000,0.000,0.051,0.022,0.007,0.000,0.080,0.013,0.018,0.000,0.012,0.000,0.000,0.015,0.027,0.000,0.009,0.007,0.044,0.007,0.058,0.000,0.028,0.021,0.000,0.000,0.013,0.000,0.020,0.022,0.002,0.000,0.044,0.071,0.009,0.013,0.006,0.000,0.104,0.068,0.000],
[0.000,0.000,0.000,0.000,0.087,0.000,0.000,0.000,1.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.012,0.000,0.063,0.000,0.000,0.000,0.000,0.012,0.000,0.012,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.022,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.021,0.000],
[0.000,0.055,0.000,0.000,0.000,0.040,0.034,0.000,0.000,1.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.024,0.049,0.000,0.000,0.004,0.000,0.000,0.000,0.000,0.000,0.070,0.000,0.010,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.014,0.000,0.011,0.000,0.000,0.000,0.043,0.000,0.040,0.010,0.000],
[0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,1.000,0.000,0.000,0.000,0.000,0.000,0.000,0.162,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.066,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.057,0.000,0.123,0.075,0.000],
[0.018,0.000,0.032,0.000,0.000,0.000,0.000,0.046,0.000,0.000,0.000,1.000,0.016,0.000,0.007,0.000,0.000,0.208,0.015,0.027,0.000,0.016,0.000,0.013,0.000,0.000,0.000,0.000,0.020,0.000,0.000,0.000,0.000,0.030,0.000,0.059,0.000,0.031,0.015,0.004,0.020,0.000,0.020,0.041,0.000,0.000,0.000,0.000,0.044,0.015,0.019,0.126,0.000,0.193,0.055,0.000],
[0.000,0.005,0.016,0.000,0.000,0.002,0.000,0.076,0.000,0.000,0.000,0.006,1.000,0.000,0.000,0.000,0.000,0.022,0.086,0.006,0.000,0.092,0.006,0.013,0.024,0.000,0.004,0.000,0.000,0.005,0.000,0.000,0.000,0.050,0.029,0.034,0.000,0.000,0.000,0.000,0.007,0.005,0.000,0.000,0.000,0.000,0.009,0.014,0.029,0.000,0.049,0.018,0.003,0.050,0.021,0.000],
[0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,1.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.500,0.000,0.000,0.000,0.000,0.000,0.000,0.000],
[0.091,0.052,0.000,0.000,0.066,0.000,0.000,0.000,0.000,0.000,0.000,0.029,0.000,0.000,1.000,0.000,0.000,0.006,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.063,0.000,0.003,0.192,0.020,0.000,0.000,0.000,0.000,0.000,0.000,0.250,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.063,0.000,0.000,0.000,0.000,0.000,0.029,0.085,0.000],
[0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.008,0.000,0.000,1.000,0.000,0.000,0.000,0.000,0.000,0.004,0.000,0.000,0.064,0.000,0.147,0.000,0.000,0.000,0.000,0.000,0.000,0.003,0.060,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.061,0.000,0.000,0.000,0.000,0.000,0.137,0.000,0.056,0.000,0.000,0.000],
[0.138,0.000,0.169,0.007,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,1.000,0.251,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.096,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.009,0.000,0.009,0.011,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.436,0.000],
[0.007,0.000,0.023,0.000,0.000,0.000,0.003,0.043,0.000,0.003,0.016,0.079,0.023,0.000,0.000,0.000,0.006,1.000,0.023,0.021,0.000,0.062,0.026,0.007,0.000,0.000,0.000,0.000,0.028,0.014,0.000,0.000,0.000,0.018,0.014,0.034,0.000,0.027,0.006,0.014,0.014,0.000,0.019,0.030,0.017,0.000,0.000,0.011,0.025,0.000,0.000,0.112,0.000,0.144,0.037,0.000],
[0.000,0.000,0.011,0.000,0.000,0.000,0.000,0.012,0.004,0.004,0.000,0.004,0.057,0.000,0.000,0.000,0.000,0.015,1.000,0.000,0.005,0.071,0.000,0.002,0.004,0.010,0.000,0.000,0.000,0.005,0.000,0.001,0.000,0.083,0.007,0.013,0.000,0.006,0.000,0.000,0.000,0.009,0.000,0.000,0.025,0.000,0.005,0.055,0.040,0.000,0.034,0.007,0.000,0.037,0.033,0.000],
[0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.040,0.000,0.000,0.000,0.071,0.040,0.000,0.000,0.000,0.000,0.147,0.000,1.000,0.000,0.035,0.000,0.001,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.040,0.000,0.066,0.000,0.110,0.069,0.000,0.000,0.000,0.000,0.065,0.000,0.000,0.000,0.000,0.005,0.000,0.000,0.059,0.000,0.133,0.006,0.000],
[0.000,0.000,0.000,0.000,0.106,0.006,0.000,0.000,0.041,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.011,0.000,1.000,0.000,0.000,0.000,0.000,0.008,0.000,0.008,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.024,0.000,0.000,0.000,0.000,0.000,0.000,0.013,0.000],
[0.000,0.000,0.009,0.000,0.000,0.000,0.000,0.038,0.000,0.000,0.000,0.003,0.052,0.000,0.000,0.000,0.000,0.034,0.061,0.003,0.000,1.000,0.002,0.011,0.010,0.004,0.000,0.000,0.000,0.011,0.000,0.000,0.000,0.068,0.018,0.044,0.000,0.006,0.025,0.000,0.004,0.006,0.000,0.006,0.033,0.000,0.008,0.027,0.064,0.000,0.033,0.026,0.003,0.072,0.016,0.000],
[0.000,0.000,0.023,0.000,0.000,0.038,0.015,0.075,0.000,0.000,0.000,0.000,0.043,0.000,0.000,0.000,0.000,0.173,0.000,0.000,0.000,0.027,1.000,0.000,0.000,0.000,0.000,0.000,0.095,0.050,0.000,0.043,0.000,0.004,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.038,0.000,0.003,0.000,0.000,0.000,0.000,0.000,0.075,0.002,0.000],
[0.000,0.000,0.000,0.029,0.000,0.000,0.000,0.059,0.000,0.000,0.000,0.019,0.048,0.000,0.000,0.000,0.000,0.026,0.009,0.000,0.000,0.077,0.000,1.000,0.016,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.031,0.085,0.000,0.010,0.008,0.000,0.023,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.079,0.021,0.000,0.000,0.000,0.018,0.018,0.021,0.000],
[0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.127,0.000,0.000,0.034,0.000,0.000,0.032,0.000,0.000,0.100,0.000,0.022,1.000,0.000,0.035,0.000,0.000,0.000,0.000,0.000,0.000,0.024,0.031,0.000,0.000,0.000,0.000,0.000,0.000,0.030,0.000,0.000,0.001,0.000,0.000,0.000,0.000,0.000,0.100,0.030,0.088,0.030,0.000,0.000],
[0.000,0.000,0.047,0.000,0.053,0.004,0.000,0.088,0.048,0.000,0.000,0.000,0.004,0.000,0.048,0.000,0.000,0.000,0.136,0.000,0.048,0.063,0.000,0.000,0.000,1.000,0.000,0.092,0.047,0.000,0.000,0.000,0.000,0.005,0.007,0.000,0.048,0.047,0.042,0.000,0.000,0.009,0.000,0.000,0.000,0.004,0.000,0.066,0.000,0.000,0.000,0.000,0.000,0.063,0.065,0.000],
[0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.031,0.000,0.000,0.124,0.000,0.000,0.000,0.000,0.000,0.001,0.000,0.000,0.056,0.000,1.000,0.000,0.000,0.000,0.000,0.000,0.000,0.038,0.092,0.000,0.000,0.000,0.000,0.000,0.044,0.000,0.000,0.000,0.075,0.000,0.069,0.000,0.000,0.070,0.085,0.000,0.096,0.000,0.000,0.000],
[0.000,0.064,0.000,0.000,0.045,0.056,0.066,0.000,0.056,0.096,0.000,0.000,0.000,0.000,0.003,0.000,0.000,0.000,0.000,0.000,0.056,0.000,0.000,0.000,0.000,0.107,0.000,1.000,0.000,0.131,0.000,0.000,0.000,0.000,0.000,0.000,0.003,0.000,0.000,0.000,0.000,0.019,0.000,0.000,0.000,0.012,0.000,0.048,0.000,0.000,0.000,0.000,0.000,0.000,0.011,0.000],
[0.135,0.000,0.052,0.000,0.000,0.020,0.007,0.044,0.000,0.000,0.000,0.026,0.000,0.000,0.061,0.000,0.008,0.098,0.000,0.000,0.000,0.000,0.050,0.000,0.000,0.019,0.000,0.000,1.000,0.000,0.057,0.023,0.036,0.026,0.000,0.000,0.020,0.067,0.000,0.028,0.001,0.000,0.000,0.003,0.000,0.020,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.012,0.055,0.000],
[0.000,0.001,0.000,0.000,0.012,0.001,0.023,0.039,0.000,0.002,0.000,0.000,0.010,0.000,0.003,0.000,0.000,0.024,0.012,0.000,0.000,0.034,0.013,0.000,0.000,0.000,0.000,0.023,0.000,1.000,0.000,0.033,0.000,0.031,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.051,0.000,0.000,0.041,0.008,0.000,0.083,0.032,0.000,0.000,0.017,0.000,0.044,0.055,0.000],
[0.060,0.000,0.045,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.228,0.000,1.000,0.000,0.103,0.036,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.094,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.094,0.000],
[0.000,0.000,0.123,0.000,0.000,0.035,0.097,0.044,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.009,0.000,0.000,0.000,0.035,0.000,0.000,0.000,0.000,0.000,0.035,0.106,0.000,1.000,0.000,0.051,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.070,0.000,0.000,0.012,0.035,0.000,0.000,0.032,0.000,0.000,0.000,0.000,0.023,0.066,0.000],
[0.018,0.000,0.000,0.126,0.000,0.000,0.000,0.066,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.088,0.000,0.000,0.000,0.000,0.109,0.000,0.079,0.000,1.000,0.015,0.000,0.000,0.075,0.060,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.060,0.060,0.000],
[0.002,0.000,0.005,0.000,0.000,0.000,0.000,0.020,0.000,0.000,0.000,0.006,0.028,0.000,0.000,0.000,0.000,0.010,0.071,0.003,0.000,0.067,0.000,0.012,0.002,0.000,0.002,0.000,0.004,0.010,0.001,0.005,0.000,1.000,0.004,0.017,0.000,0.003,0.010,0.000,0.000,0.010,0.000,0.003,0.029,0.000,0.003,0.057,0.030,0.004,0.002,0.009,0.000,0.028,0.002,0.000],
[0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.013,0.000,0.000,0.000,0.000,0.069,0.000,0.000,0.014,0.000,0.033,0.024,0.000,0.000,0.075,0.000,0.000,0.014,0.002,0.026,0.000,0.000,0.000,0.000,0.000,0.000,0.017,1.000,0.045,0.013,0.000,0.000,0.000,0.010,0.000,0.000,0.000,0.012,0.000,0.013,0.000,0.043,0.018,0.073,0.027,0.013,0.088,0.075,0.000],
[0.000,0.000,0.020,0.000,0.000,0.003,0.000,0.079,0.000,0.000,0.000,0.036,0.057,0.000,0.000,0.000,0.000,0.054,0.033,0.015,0.000,0.129,0.000,0.004,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.050,0.031,1.000,0.000,0.013,0.028,0.001,0.009,0.018,0.000,0.022,0.016,0.000,0.000,0.007,0.101,0.011,0.009,0.024,0.000,0.127,0.060,0.000],
[0.000,0.027,0.000,0.022,0.076,0.000,0.000,0.000,0.058,0.000,0.000,0.001,0.000,0.000,0.129,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.013,0.000,0.032,0.000,0.001,0.032,0.000,0.000,0.000,0.040,0.000,0.032,0.000,1.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.067,0.000],
[0.002,0.000,0.046,0.022,0.000,0.000,0.000,0.093,0.000,0.000,0.000,0.046,0.004,0.000,0.000,0.000,0.000,0.105,0.039,0.061,0.000,0.044,0.000,0.000,0.000,0.022,0.000,0.000,0.074,0.000,0.000,0.000,0.022,0.025,0.000,0.032,0.000,1.000,0.045,0.066,0.002,0.022,0.000,0.024,0.000,0.000,0.000,0.067,0.000,0.000,0.028,0.000,0.000,0.098,0.097,0.000],
[0.000,0.000,0.029,0.000,0.000,0.000,0.000,0.093,0.000,0.000,0.000,0.029,0.000,0.000,0.000,0.000,0.000,0.032,0.005,0.050,0.000,0.228,0.000,0.032,0.000,0.025,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.092,0.000,0.088,0.000,0.060,1.000,0.000,0.024,0.000,0.000,0.027,0.000,0.000,0.000,0.017,0.237,0.000,0.000,0.073,0.000,0.152,0.031,0.000],
[0.203,0.000,0.010,0.083,0.000,0.000,0.000,0.000,0.000,0.000,0.097,0.024,0.000,0.000,0.000,0.000,0.003,0.202,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.120,0.000,0.000,0.000,0.000,0.000,0.000,0.010,0.000,0.250,0.000,1.000,0.006,0.000,0.000,0.014,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.031,0.004,0.000],
[0.011,0.000,0.007,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.065,0.063,0.000,0.000,0.000,0.002,0.117,0.000,0.000,0.000,0.070,0.000,0.000,0.000,0.000,0.046,0.000,0.003,0.000,0.000,0.000,0.000,0.000,0.037,0.050,0.000,0.003,0.041,0.003,1.000,0.000,0.049,0.018,0.000,0.000,0.000,0.000,0.051,0.076,0.014,0.059,0.000,0.036,0.001,0.000],
[0.000,0.000,0.047,0.000,0.000,0.007,0.016,0.045,0.000,0.000,0.000,0.000,0.023,0.000,0.000,0.000,0.000,0.001,0.059,0.000,0.000,0.047,0.000,0.000,0.023,0.004,0.000,0.008,0.000,0.120,0.000,0.051,0.000,0.074,0.000,0.045,0.000,0.023,0.000,0.000,0.000,1.000,0.000,0.000,0.000,0.000,0.000,0.114,0.045,0.015,0.023,0.000,0.000,0.017,0.033,0.000],
[0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.111,0.000,0.000,0.000,0.000,0.000,0.286,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.083,0.000,1.000,0.015,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.213,0.000,0.044,0.000,0.000],
[0.000,0.000,0.057,0.000,0.000,0.000,0.000,0.172,0.000,0.000,0.000,0.156,0.000,0.000,0.000,0.000,0.000,0.305,0.000,0.094,0.000,0.108,0.000,0.000,0.000,0.000,0.000,0.000,0.010,0.000,0.068,0.000,0.000,0.057,0.000,0.139,0.000,0.063,0.054,0.010,0.021,0.000,0.010,1.000,0.000,0.000,0.000,0.000,0.115,0.000,0.000,0.092,0.000,0.345,0.120,0.000],
[0.000,0.000,0.011,0.000,0.000,0.000,0.000,0.035,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.011,0.000,0.031,0.072,0.000,0.000,0.111,0.000,0.000,0.000,0.000,0.017,0.000,0.000,0.045,0.000,0.004,0.000,0.099,0.010,0.018,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,1.000,0.000,0.000,0.085,0.043,0.000,0.021,0.033,0.000,0.052,0.011,0.000],
[0.000,0.050,0.034,0.000,0.056,0.119,0.023,0.014,0.000,0.019,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.056,0.000,0.000,0.004,0.000,0.012,0.056,0.047,0.000,0.063,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,1.000,0.000,0.062,0.000,0.000,0.000,0.000,0.000,0.000,0.057,0.066],
[0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.071,0.000,0.000,0.000,0.000,0.000,0.062,0.000,0.000,0.116,0.000,0.000,0.000,0.000,0.064,0.000,0.000,0.000,0.000,0.000,0.000,0.043,0.043,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,1.000,0.000,0.000,0.000,0.055,0.000,0.020,0.000,0.049,0.000],
[0.000,0.002,0.019,0.000,0.006,0.002,0.007,0.041,0.000,0.002,0.000,0.000,0.016,0.000,0.006,0.000,0.000,0.012,0.094,0.000,0.018,0.052,0.000,0.023,0.000,0.008,0.000,0.005,0.000,0.053,0.000,0.000,0.000,0.113,0.000,0.005,0.000,0.019,0.004,0.000,0.000,0.030,0.000,0.000,0.050,0.007,0.000,1.000,0.021,0.000,0.000,0.014,0.000,0.031,0.014,0.000],
[0.000,0.000,0.019,0.000,0.000,0.000,0.005,0.080,0.000,0.000,0.000,0.022,0.039,0.007,0.000,0.000,0.000,0.033,0.081,0.001,0.000,0.153,0.000,0.007,0.000,0.000,0.000,0.000,0.000,0.025,0.000,0.008,0.000,0.074,0.025,0.083,0.000,0.000,0.061,0.000,0.008,0.015,0.000,0.015,0.030,0.000,0.000,0.026,1.000,0.000,0.012,0.057,0.000,0.118,0.043,0.000],
[0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.075,0.000,0.000,0.000,0.055,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.082,0.000,0.000,0.000,0.000,0.000,0.000,0.073,0.078,0.070,0.000,0.000,0.000,0.000,0.085,0.037,0.000,0.000,0.000,0.000,0.000,0.000,0.000,1.000,0.067,0.000,0.000,0.000,0.026,0.000],
[0.000,0.000,0.010,0.000,0.000,0.000,0.000,0.016,0.000,0.000,0.000,0.010,0.071,0.000,0.000,0.019,0.000,0.000,0.076,0.000,0.000,0.086,0.000,0.000,0.027,0.000,0.014,0.000,0.000,0.000,0.000,0.000,0.000,0.006,0.044,0.008,0.000,0.010,0.000,0.000,0.002,0.008,0.000,0.000,0.016,0.000,0.010,0.000,0.013,0.010,1.000,0.024,0.010,0.033,0.010,0.000],
[0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.008,0.000,0.009,0.008,0.071,0.028,0.000,0.000,0.000,0.000,0.167,0.017,0.013,0.000,0.070,0.000,0.000,0.008,0.000,0.000,0.000,0.000,0.015,0.000,0.000,0.000,0.026,0.017,0.023,0.000,0.000,0.021,0.000,0.010,0.000,0.021,0.014,0.027,0.000,0.000,0.019,0.064,0.000,0.025,1.000,0.008,0.156,0.037,0.000],
[0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.040,0.000,0.000,0.084,0.000,0.000,0.000,0.000,0.000,0.084,0.000,0.070,0.250,0.000,0.173,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.085,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.040,0.000,0.000,0.000,0.103,0.084,1.000,0.084,0.000,0.000],
[0.005,0.000,0.026,0.000,0.000,0.000,0.004,0.068,0.000,0.004,0.009,0.057,0.040,0.000,0.002,0.000,0.000,0.111,0.045,0.015,0.000,0.102,0.009,0.004,0.004,0.006,0.000,0.000,0.003,0.020,0.000,0.003,0.004,0.039,0.029,0.061,0.000,0.019,0.023,0.002,0.003,0.003,0.002,0.026,0.022,0.000,0.000,0.022,0.070,0.000,0.018,0.081,0.004,1.000,0.046,0.000],
[0.002,0.004,0.037,0.000,0.014,0.000,0.000,0.047,0.008,0.001,0.006,0.017,0.018,0.000,0.006,0.000,0.008,0.030,0.042,0.000,0.007,0.023,0.000,0.005,0.000,0.006,0.000,0.000,0.013,0.026,0.005,0.010,0.005,0.002,0.026,0.030,0.010,0.020,0.005,0.000,0.000,0.007,0.000,0.010,0.005,0.005,0.005,0.010,0.027,0.002,0.006,0.020,0.000,0.048,1.000,0.000],
[0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.005,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.082,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.002,1.000]])

for i in range(M.ncols()):
	M[i,i] = 0.0

G = DiGraph(M)