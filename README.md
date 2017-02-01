# ahp_labs

## LAB1

Development environment: VS2013

variant:
  - Size of matrix: 8x8
  - Data type: double
  - With sse2 vectorization speed gain: about 40-50%

How to start vectorization mode:
project --> propirties --> configurations -->c/c++ --> code generation -->
instructions superset --> choose sse/sse2 mode(on) or IA32(off)

Before it is necessary to change optimization settings:
project --> propirties --> configurations -->c/c++ --> optimization --> 
choose full optimization(Ox)
