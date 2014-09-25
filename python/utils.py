##!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
from __future__ import print_function

import math


def linspace(start, end, n_values=None, step=None):
    """Function that generates a list of numbers. Specify one
       of n_values or step, but not both.

       If n_values is specified, generates a linearly-spaced
       list of 'n_values' numbers from 'start' to 'end'.

       If step is specified, generates a list of numbers
       from 'start' to 'end' separated by 'step'."""
    if (n_values is None and step is None) or \
       (n_values is not None and step is not None):
        raise RuntimeError("You must specify one (and only one) of n_values or step.")

    diff = step
    num_vals = n_values
    if n_values is not None:
        if n_values < 2:
            return [end]
        diff = (float(end) - start)/(n_values - 1)
    else:
        num_vals = int(((end - start) / step) + 1)
    return [diff * i + start  for i in range(num_vals)]


def get_frequency_list(freq_start_hz, freq_end_hz,
                       freq_step=None, points_per_octave=None):
    """Builds a list of frequencies from freq_start_hz to
       freq_end_hz.  Caller must specify either freq_step or
       points_per_octave.
       
       If freq_step is specified, the list is simply a linear
       list of frequencies from start to end incrementing by
       the step value.
       
       If points_per_octave is specified, the frequency is stepped
       by octaves, with the appropriate number of points in
       between."""
    if (freq_step is None and points_per_octave is None) or \
       (freq_step is not None and points_per_octave is not None):
        raise RuntimeError("You must specify one (and only one) of freq_step or points_per_octave.")

    # If using a step value, just use linspace as-is
    if freq_step is not None:
        return linspace(freq_start_hz, freq_end_hz, step=freq_step)

    # If using points per octave, things get a little more challenging and
    # we build up the list with a call to linspace for each octave
    freqs = []
    f = freq_start_hz
    if points_per_octave is not None:
        while f < freq_end_hz:
            freqs.extend([w for w in linspace(f, 2*f, n_values=points_per_octave+1) if w not in freqs])
            f = 2*f

    return [w for w in freqs if (w >= freq_start_hz and w <= freq_end_hz)]

if __name__ == "__main__":
    # print(linspace(0, 10, step=1))
    # print(linspace(4, 8, step=1))
    # print(linspace(4, 8, step=0.25))
    # print(linspace(1, 10, n_values=1))
    
    # print(get_frequency_list(200, 20000, freq_step=100))
    # print(get_frequency_list(10, 20000, freq_step=100))
    # print(get_frequency_list(200, 20000, freq_step=333))
    # print(get_frequency_list(200, 20000, points_per_octave=3))
    # print(get_frequency_list(10, 100000, points_per_octave=5))
    # print(get_frequency_list(10, 100000, points_per_octave=10))
    # print(get_frequency_list(1, 100000, points_per_octave=10))
    print(get_frequency_list(100, 10000, points_per_octave=20))
