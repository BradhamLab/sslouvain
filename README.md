**Notice** 

This package is a fork of the no-longer-maintained [`louvain-igraph`](https://github.com/vtraag/louvain-igraph) package that has been superseded by the [leidenalg](https://github.com/vtraag/leidenalg) package. This package implements a semi-supervised version of Louvain community detection, where specified labels remain constant during optimization. The API is consistent with the original `louvain-igraph` package, and functionality and implementation should be unchanged. 

sslouvain
================

This package implements a semi-superised version of the louvain algorithm in `C++` and exposes it to
`python`.  It relies on `(python-)igraph` for it to function. Besides the
relative flexibility of the implementation, it also scales well, and can be run
on graphs of millions of nodes (as long as they can fit in memory). The core
function is `find_partition` which finds the optimal partition using the
louvain algorithm [1] for a number of different methods. The methods currently
implemented are (1) modularity [2], (2) Reichardt and Bornholdt's model using
the configuration null model and the Erdös-Rényi null model [3], (3) the
constant Potts model (CPM) [4], (4) Significance [5], and finally (5)
Surprise [6]. In addition, it supports multiplex partition optimisation
allowing community detection on for example negative links [7] or multiple
time slices [8]. It also provides some support for community detection on
bipartite graphs. See the [documentation](http://louvain-igraph.readthedocs.io/en/latest/) for more information.

![Louvain Documentation](https://readthedocs.org/projects/louvain-igraph/badge "Louvain Documentation Status")
![Louvian Build Status](https://travis-ci.org/vtraag/louvain-igraph.svg "Louvain Build Status")
![DOI](https://zenodo.org/badge/31305324.svg "DOI")

Installation
------------

Currently, the package must be built locally. First, clone the repo and build the package by issueing the following commands from the repo head:

```bash
python setup.py build
python setup.py install
```

In short, for Unix: `pip install sslouvain`. For Windows: download the binary
installers.

For Unix like systems it is possible to install from source. For Windows this
is overly complicated, and you are recommended to use the binary installation
files.  There are two things that are needed by this package: the igraph `C`
core library and the python-igraph python package. For both, please see
http://igraph.org.

Make sure you have all necessary tools for compilation. In Ubuntu this can be
installed using `sudo apt-get install build-essential`, please refer to the
documentation for your specific system.  Make sure that not only `gcc` is
installed, but also `g++`, as the louvain package is programmed in `C++`.
Note that to compile `igraph` itself, you also need to install
`libxml2-dev`.

You can check if all went well by running a variety of tests using `python
setup.py test`.

There are basically two installation modes, similar to the python-igraph
package itself (from which most of the setup.py comes).

1. No `C` core library is installed yet. The packages will be compiled and
   linked statically to an automatically downloaded version of the `C` core
   library of igraph.
2. A `C` core library is already installed. In this case, the package will
   link dynamically to the already installed version. This is probably also the
   version that is used by the igraph package, but you may want to double check
   this.

In case the python-igraph package is already installed before, make sure that
both use the **same versions**.

The cleanest setup it to install and compile the `C` core library yourself
(make sure that the header files are also included, e.g. install also the
development package from igraph). Then both the python-igraph package, as well
as this package are compiled and (dynamically) linked to the same `C` core
library.

Troubleshooting
---------------

In case of any problems, best to start over with a clean environment. Make sure
you remove the python-igraph package completely, remove the `C` core library
and remove the louvain package. Then, do a complete reinstall starting from
`pip install sslouvain`. In case you want a dynamic library be sure to then
install the `C` core library from source before. Make sure you **install the
same versions**.

Usage
-----

There is no standalone version of louvain-igraph, and you will always need
python to access it. There are no plans for developing a standalone version or
R support. So, use python. Please refer to the documentation for more details
on function calls and parameters.

Just to get you started, below the essential parts.
To start, make sure to import the packages:

```python
import sslouvain
import igraph as ig
```

We'll create a random graph for testing purposes:

```python
G = ig.Graph.Erdos_Renyi(100, 0.1)
```

For finding a partition using traditional louvain community detection:

```python
part = sslouvain.find_partition(G)
```

However, by specifiying both initial membership and mutable nodes, we can perform semi-supervised clustering.

```python

import random

# label first half of nodes
labels = random.choices(range(5), k= G.vcount() // 2)
labels += list(range(5, G.vcount() // 2))

# set first half of nodes as immutable
mutable = [False] * G.vcount() // 2 + [True] * G.vcount() // 2

part = sslouvain.find_partition(G
                                initial_membership=labels,
                                mutable_nodes=mutable)
```

Tests
-----

To test for specific semi-supervised implementation, from the head of the repository run:
```
python tests/test_SemiSupervised.py
```

To run original `louvain` unit tests for optimization and vertex partitions:

```
python tests/test_VertexPartition.py
python tests/test_Optimiser.py
```

**Semi-supervised multiplex and bipartite partitioning is un-tested.**


References
----------

Please cite the references appropriately in case they are used.



1. Blondel, V. D., Guillaume, J.-L., Lambiotte, R., & Lefebvre, E. (2008).
    Fast unfolding of communities in large networks. Journal of Statistical
    Mechanics: Theory and Experiment, 10008(10), 6.
    [10.1088/1742-5468/2008/10/P10008](http://doi.org/10.1088/1742-5468/2008/10/P10008)

2. Newman, M. E. J., & Girvan, M. (2004). Finding and evaluating community
    structure in networks. Physical Review E, 69(2), 026113.
    [10.1103/PhysRevE.69.026113](http://doi.org/10.1103/PhysRevE.69.026113)

3. Reichardt, J., & Bornholdt, S. (2006). Statistical mechanics of
    community detection. Physical Review E, 74(1), 016110.
    [10.1103/PhysRevE.74.016110](http://doi.org/10.1103/PhysRevE.74.016110)

4. Traag, V. A., Van Dooren, P., & Nesterov, Y. (2011). Narrow scope for
    resolution-limit-free community detection. Physical Review E, 84(1),
    016114. [10.1103/PhysRevE.84.016114](http://doi.org/10.1103/PhysRevE.84.016114)

5. Traag, V. A., Krings, G., & Van Dooren, P. (2013). Significant scales in
    community structure. Scientific Reports, 3, 2930. [10.1038/srep02930](http://doi.org/10.1038/srep02930)

6. Traag, V. A., Aldecoa, R., & Delvenne, J.-C. (2015). Detecting
    communities using asymptotical surprise. Physical Review E, 92(2),
    022816. [10.1103/PhysRevE.92.022816](http://doi.org/10.1103/PhysRevE.92.022816)

7. Traag, V. A., & Bruggeman, J. (2009). Community detection in networks
    with positive and negative links. Physical Review E, 80(3), 036115.
    [10.1103/PhysRevE.80.036115](http://doi.org/10.1103/PhysRevE.80.036115)

8. Mucha, P. J., Richardson, T., Macon, K., Porter, M. A., & Onnela, J.-P.
    (2010). Community structure in time-dependent, multiscale, and multiplex
    networks. Science, 328(5980), 876–8. [10.1126/science.1184819](http://doi.org/10.1126/science.1184819)

Licence
-------

Copyright (C) 2020 D.Y. Hawkins 2016 V.A. Traag

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see http://www.gnu.org/licenses/.
