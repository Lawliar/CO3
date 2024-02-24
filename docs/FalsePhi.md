## Why
- I introduce False Phi Root/Leaves for the purpose of supporting the short circuit functionality from SymCC. This seems like a rather important component of it. 
- SymCC's short circuit is to deal with situations when constructing a symbolic operation, there are mixed input states (e.g., some inputs are symbolic while others are some concrete; or all symbolic; or all concrete). 
    - When all inputs are concrete, its result will be concrete (i.e., false). 
    - When all inputs are symbolic, its result will be the symbolic calculations for all inputs.
    - When some are concrete, some are symbolic, however, the result will also be symbolic, and those concrte inputs need to be symbolized. And this is where the short circuit logic comes into play. 
        - It will symbolize those concrete inputs with its concrete values. And construct the final symbolic results based on the symbolized inputs. 
- Since SymCC achieved this functionality through artificially inserting Phi Node: depending on the control flow through the artificially inserted branches, it can take different values to reflect the different situations described above. 
- CO3 of course can treat them as legit Phi Nodes and report their choice at runtime, but this is costly considering how frequent this process is used. 
- Thus, we introduce the concept of False Phi node to differentiate from the True Phi Node that we need to report at runtime. 
- The reason behind this is just, without any reporting, we know at the workstation, about the input states for all the inputs. Thus, we only need to check the states for all the inputs and decide at workstation. If any symbolization of the concrete input is needed, its concrete value to symbolize will be reported. 

## How:
1. We name the result of the symbolic computation, FalsePhiRoot, and their inputs, FalsePhiLeaf. A better name might be helpful. 
2. For both false phi root and false phi leaf, there are at least two in-edges annoted with 0, and 1, as they are originally Phi Nodes with two different values to take from. 
3. Although both false phi leaf and root share this annotation, their meaning is kind of different:
    - For false phi root
        - 0-annotated edge always originated from false (i.e., concrete), as this is the "fast path" where all inputs are concrete.
        - 1-edge is the "slow path" where at least one input is symbolic, so all inputs have to be symbolized, and it has to go through the symbolic computation, this edge originates from that value. 
    - For false phi leaf:
        - the 0-edge is its original value (i.e., if its symbolic).
        - the 1-edge is its new value (i.e., when its original value is concrete, but it has to be symbolized. )
4. Although 0-edge and 1-edge are bound to be singular, there can be multiple 2-annotated edges. 
    - For false phi root, they are the dependent false phi leaves (the input values).
    - For false phi leaves, they are its peers. 
        - This annotation enables false phi leaves to be aware of the states of other inputs. So that it can decide which value to choose from. 

## Conclusion:
With this, we managed to construct this Phi-node based short circuit logic without any reporting. 

 