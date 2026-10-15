# V182: public pose quaternion normalization

One Source header overlay over V181; SDK remains V181. Replace the solver's double sum with the public setter's individual float squares and X,Y,Z,W float accumulation, square root and reciprocal. This intermediate correction preserves upright cases and removes a tilted normalization discrepancy. V183 additionally corrects the public matrix decoder and preserves stored quaternion on an exactly unchanged pose. Final acceptance uses V184; intermediate proposal tests are not whole-world acceptance.
