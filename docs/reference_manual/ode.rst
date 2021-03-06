======================
Differential Equations
======================

In this chapter, some facilities for solving differential equations
are described. Currently only simple equations without auxiliary
conditions are supported.


.. function:: OdeSolve(expr1==expr2)

   general ODE solver

   :param expr1,expr2: expressions containing a function to solve for

   This function currently can solve second order homogeneous linear equations
   with real constant  coefficient. The solution is returned with unique
   constants  generated by {UniqueConstant}. The roots of the auxiliary
   equation are used as the arguments of exponentials. If the roots are complex
   conjugate  pairs, then the solution returned is in the form of exponentials,
   sines  and cosines. First and second derivatives are entered as ``y'``,
   ``y''``. Higher order derivatives  may be entered as ``y(n)``, where ``n``
   is any positive integer.

   :Example:

   ::

      In> OdeSolve( y'' + y == 0 )
      Out> C42*Sin(x)+C43*Cos(x);
      In> OdeSolve( 2*y'' + 3*y' + 5*y == 0 )
      Out> Exp(((-3)*x)/4)*(C78*Sin(Sqrt(31/16)*x)+C79*Cos(Sqrt(31/16)*x));
      In> OdeSolve( y'' - 4*y == 0 )
      Out> C132*Exp((-2)*x)+C136*Exp(2*x);
      In> OdeSolve( y'' +2*y' + y == 0 )
      Out> (C183+C184*x)*Exp(-x);


   .. seealso:: :func:`Solve`, :func:`RootsWithMultiples`


.. function:: OdeTest(eqn,testsol)

   test the solution of an ODE

   :param eqn: equation to test
   :param testsol: test solution

   This function automates the verification of the solution of an ODE.
   It can also be used to quickly see how a particular equation
   operates  on a function.

   :Example:

   ::

      In> OdeTest(y''+y,Sin(x)+Cos(x))
      Out> 0;
      In> OdeTest(y''+2*y,Sin(x)+Cos(x))
      Out> Sin(x)+Cos(x);


   .. seealso:: :func:`OdeSolve`


.. function:: OdeOrder(eqn)

   return order of an ODE

   :param eqn: equation

   This function returns the order of the differential equation, which
   is  order of the highest derivative. If no derivatives appear, zero
   is returned.

   :Example:

   ::

      In> OdeOrder(y'' + 2*y' == 0)
      Out> 2;
      In> OdeOrder(Sin(x)*y(5) + 2*y' == 0)
      Out> 5;
      In> OdeOrder(2*y + Sin(y) == 0)
      Out> 0;


   .. seealso:: :func:`OdeSolve`

.. function:: WronskianMatrix(func,var)

   create the Wronskian matrix

   :param func: an :math:`n`-dimensional vector of functions
   :param var: a variable to differentiate with respect to

   The function :func:`WronskianMatrix` calculates the `Wronskian matrix`_  of :math:`n`
   functions. The Wronskian matrix is created by putting each function as the
   first element of each column, and filling in the rest of each  column by the
   :math:`(i-1)`-th derivative, where :math:`i` is the current row.    The
   Wronskian matrix is used to verify that the :math:`n` functions are linearly
   independent, usually solutions to a differential equation.  If the
   determinant of the Wronskian matrix is zero, then the functions  are
   dependent, otherwise they are independent.

   :Example:

   ::

      In> WronskianMatrix({Sin(x),Cos(x),x^4},x);
      Out> {{Sin(x),Cos(x),x^4},{Cos(x),-Sin(x),4*x^3},
      {-Sin(x),-Cos(x),12*x^2}};
      In> PrettyForm(%)
      /                                                 \
      | ( Sin( x ) )      ( Cos( x ) )      /  4 \      |
      |                                     \ x  /      |
      |                                                 |
      | ( Cos( x ) )      ( -( Sin( x ) ) ) /      3 \  |
      |                                     \ 4 * x  /  |
      |                                                 |
      | ( -( Sin( x ) ) ) ( -( Cos( x ) ) ) /       2 \ |
      |                                     \ 12 * x  / |
      \                                                 /

   The last element is a linear combination of the first two, so the determinant is zero::

      In> A:=Determinant( WronskianMatrix( {x^4,x^3,2*x^4+3*x^3},x ) )
      Out> x^4*3*x^2*(24*x^2+18*x)-x^4*(8*x^3+9*x^2)*6*x
      +(2*x^4+3*x^3)*4*x^3*6*x-4*x^6*(24*x^2+18*x)+x^3
      *(8*x^3+9*x^2)*12*x^2-(2*x^4+3*x^3)*3*x^2*12*x^2;
      In> Simplify(A)
      Out> 0;

.. _Wronskian matrix: https://en.wikipedia.org/wiki/Wronskian
