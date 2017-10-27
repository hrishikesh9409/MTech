#! /usr/bin/python

def power(x, n, id, sqr, mul):
  """
  Compute and return X^N for nonnegative N.

  This is an abstract operation, assuming only that X is an element of a
  monoid: ID is the multiplicative identity, and the necessary operations are
  provided as functions: SQR(Y) returns Y^2, and MUL(Y, Z) returns the
  product Y Z.
  """

  ## First, we need to find a 2^{2^i} > n.
  i = 1
  while (n >> i) > 0: i = 2*i

  ## Refine our estimate, so that 2^{k-1} <= n < 2^k.
  k, m = 1, n
  while m > 1:
    i = i >> 1
    mm = m >> i
    if mm > 0: m, k = mm, k + i

  ## Now do the square-and-multiply thing.
  y = id
  for i in xrange(k - 1, -1, -1):
    y = sqr(y)
    if (n >> i)%2: y = mul(x, y)

  ## We're done.
  return y

def fibonacci(n):
  """
  Compute and return the Nth Fibonacci number.

  Let F_0 = 0, F_1 = 1, F_{k+1} = F_k + F_{k-1}, for all integers k.  Note
  that this is well-defined even for negative k.  We return F_N.
  """

  ## Auxiliary functions for working in our polynomial ring.
  def poly_sqr((a, b)):
    a2 = a*a
    return 2*a*b + a2, a2 + b*b
  def poly_mul((a, b), (c, d)):
    ac = a*c
    return a*d + b*c + ac, ac + b*d

  ## Do the job.  For negative indices, we take powers of t^{-1}.
  if n < 0: return power((1, -1), -n, (0, 1), poly_sqr, poly_mul)
  else: return power((1, 0), n, (0, 1), poly_sqr, poly_mul)

## Command-line interface.
if __name__ == '__main__':
  import sys
  #try: n = (lambda _, n: int(n))(*sys.argv)
  try: n = input()
  except TypeError, ValueError:
    print >>sys.stderr, 'Usage: fib N'
    sys.exit(1)
  #print fibonacci(n)[0]
  ans = fibonacci(n)[0]
  print ans