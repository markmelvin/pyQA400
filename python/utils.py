##!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
import math

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
        raise RuntimeError, "You must specify one (and only one) of freq_step or points_per_octave."

    # Create a list of frequencies to test
    # (we don't use list comprehensions or range so we can support
    #  floating point step values)
    freqs = []
    f = freq_start_hz

    if freq_step is not None:
        while f <= freq_end_hz:
            freqs.append(f)
            f += freq_step

    if points_per_octave is not None:
        _exp = 0
        while _exp < math.ceil(math.log10(freq_end_hz)):
            f = 10**_exp
            end = 10**(_exp+1)
            step = float(end) / float(points_per_octave)
            while f <= end:
                if not f in freqs:
                    freqs.append(f)
                f += step
            _exp += 1

    return [w for w in freqs if (w >= freq_start_hz and w <= freq_end_hz)]

if __name__ == "__main__":
    print get_frequency_list(200, 20000, freq_step=100)
    print get_frequency_list(10, 20000, freq_step=100)
    print get_frequency_list(200, 20000, freq_step=333)
    print get_frequency_list(200, 20000, points_per_octave=3)
    print get_frequency_list(10, 100000, points_per_octave=5)
    print get_frequency_list(10, 100000, points_per_octave=10)
    print get_frequency_list(1, 100000, points_per_octave=10)
