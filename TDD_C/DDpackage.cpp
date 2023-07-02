/*
 * This file is part of IIC-JKU DD package which is released under the MIT license.
 * See file README.md or go to http://iic.jku.at/eda/research/quantum_dd/ for more information.
 */


#include "DDpackage.h"
#include <limits>
#include <unordered_set>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cmath>

namespace dd_package {
    constexpr unsigned int NODECOUNT_BUCKETS = 2000000;
    const complex_value complex_one = {1, 0};
    const complex_value complex_zero = {0, 0};
    const long double sqrt_2 = 1.0L / std::sqrt(2.0L);
    int cache_num = 2000;
    bool to_test = false;

    // NOT operations
    const DD_matrix Nm = {{complex_zero, complex_one},
                          {complex_one,  complex_zero}};
    // phase shift S
    const DD_matrix Sm = {{complex_one,  complex_zero},
                          {complex_zero, {0, 1}}};
    // Hadamard
    const DD_matrix Hm = {{{sqrt_2, 0}, {sqrt_2,  0}},
                          {{sqrt_2, 0}, {-sqrt_2, 0}}};
    // phase shift Z = S*S
    const DD_matrix Zm = {{complex_one,  complex_zero},
                          {complex_zero, {-1, 0}}};

    const DD_matrix Im= { {complex_one, complex_zero }, { complex_zero, complex_one} };


    const DD_matrix Ym = { {complex_zero, { 0, 1 } }, {{ 0, -1 }, complex_zero } };

    const DD_matrix Sdgm = { {complex_one, complex_zero }, {complex_zero, { 0, -1 } } };
    const DD_matrix Tm = { {complex_one, complex_zero }, {complex_zero, { sqrt_2, sqrt_2} } };
    const DD_matrix Tdgm = { {complex_one, complex_zero }, {complex_zero, { sqrt_2, -sqrt_2 } } };


    const int Radix = MAXRADIX;                 // radix (default is 2)
    const int Nedge = MAXNEDGE;                 // no. of edges (default is 4)
    int GCswitch = 1;                           // set switch to 1 to enable garbage collection
    int Smode = 1;                              // S mode switch for spectral transformation
    int RMmode = 0;                             // Select RM transformation mode
    int MultMode = 0;                           // set to 1 for matrix - vector multiplication

    int RenormalizationNodeCount = 0;           // number of active nodes that need renormalization (used in DDdecRef)
    int blockMatrixCounter = 0;                 // number of active nodes that represent block matrices (used in DDincRef, DDdecRef)
    int globalComputeSpecialMatricesFlag = 1;   // default value for computeSpecialMatricesFlag of newly created nodes (used in DDmakeNonterminal)

    DDnodePtr Avail;                            // pointer to available space chain
    ListElementPtr Lavail;                      // pointer to available list elements for breadth first searchess
    DDnodePtr DDterminalNode;                   // pointer to terminal node
    DDedge DDone, DDzero;                       // edges pointing to zero and complex_one DD constants
    long DDorder[MAXN];                         // variable order initially 0,1,... from bottom up | Usage: DDorder[level] := varible at a certain level
    long DDinverseOrder[MAXN];                  // inverse of variable order (inverse permutation) | Usage: DDinverseOrder[variable] := level of a certain variable
    long DDnodecount;                           // counts active nodes
    long DDpeaknodecount;                       // records peak node count in unique table
    long Nop[6];                                // operation counters
    long CTlook[20], CThit[20];                 // counters for gathering compute table hit stats
    long UTcol, UTmatch, UTlookups;             // counter for collisions / matches in hash tables
    int GCcurrentLimit;                         // current garbage collection limit
    int ComplexCurrentLimit;                    // current garbage collection limit
    int ActiveNodeCount;                        // number of active nodes
    int Active[MAXN];                           // number of active nodes for each variable
    DDedge DDnullEdge;                          // set in DDinit routine
    int PermList[MAXPL];                        // array for recording a permutation
    DDnodePtr Unique[MAXN][NBUCKET];
    CTentry CTable1[CTSLOTS];
    CTentry2 CTable2[CTSLOTS];


    std::map<std::string, int> varOrder;

    DDedge DDid[MAXN];
    int Nlabel;        // number of labels
    char Label[MAXN][MAXSTRLEN];  // label table

    int CacheSize() {
        complex_table_entry *p = ComplexCache_Avail;
        int size = 0;

        intptr_t min = std::numeric_limits<intptr_t>::max();
        intptr_t max = std::numeric_limits<intptr_t>::min();

        while (p != nullptr && size <= 1800) {
            if (p->ref != 0) {
                std::cerr << "Entry with refcount != 0 in Cache!\n";
                std::cerr << (intptr_t) p << " " << p->ref << " " << p->val << " " << (intptr_t) p->next << "\n";
            }
            if (((intptr_t) p) < min) {min = (intptr_t) p;}
            if (((intptr_t) p) > max) {max = (intptr_t) p;}

            p = p->next;
            size++;
        }
        if (size > 1800) {
            p = ComplexCache_Avail;
            for (int i = 0; i < 10; i++) {
                std::cout << i << ": " << (uintptr_t) p << "\n";
                p = p->next;
            }
            std::cerr << "Error in Cache!\n" << std::flush;
            std::exit(1);
        }
        std::cout << "Min ptr in cache: " << min << ", max ptr in cache: " << max << "\n";
        return size;
    }

    // for debugging purposes - not normally used
    void DDdebugnode(DDnodePtr p) {
       if (p == DDzero.p) {
            std::cout << "terminal\n";
            return;
        }
        std::cout  <<"Debug node" << (intptr_t) p << "\n";
        std::cout << "node v " << (int) DDorder[p->v] <<" (" << (int) p->v << ") edges (w,p) ";
        for (auto const & i : p->e) {
            std::cout << i.w << " " << (intptr_t) i.p <<" || ";
        }
        std::cout << "ref " << p->ref << "\n";
    }

    ListElementPtr DDnewListElement() {
        ListElementPtr r;

        if (Lavail != nullptr) {   // get node from avail chain if possible
            r = Lavail;
            Lavail = Lavail->next;
        } else {            // otherwise allocate 2000 new nodes
            r = new ListElement[2000];
            ListElementPtr r2 = r + 1;
            Lavail = r2;
            for (int i = 0; i < 1998; i++, r2++) {
                r2->next = r2+1;
            }
            r2->next = nullptr;
        }
        return r;
    }

    // a slightly better DD print utility
    void DDprint(DDedge e, unsigned int limit) {
        ListElementPtr first, q, lastq, pnext;
        uint64_t n, i, j;

        first = DDnewListElement();
        first->p = e.p;
        first->next = nullptr;

        first->w = 0;
        first->cnt = 1;
        n = 0;
        i = 0;
        std::cout << "top edge weight " << e.w << "\n";
        pnext = first;

        while (pnext != nullptr) {
            std::cout << pnext->cnt << " " << pnext->p->ref;
            std::cout << (pnext->p->block ? 'B' : ' ');
            std::cout << (pnext->p->diag ? 'D' : ' ');
            std::cout << (pnext->p->ident ? 'I' : ' ');
            std::cout << (pnext->p->symm ? 'S' : ' ');

            if (pnext->p->renormFactor != COMPLEX_ONE)
                std::cout << "R=" << pnext->p->renormFactor;
            else
                std::cout << "    ";
            std::cout << i << "|  (" << pnext->p->v << ")";

            std::cout << "[";
            if (pnext->p != DDzero.p) {
                for (j = 0; j < Nedge; j++) {
                    if (pnext->p->e[j].p == nullptr) {
                        std::cout << "NULL ";
                    } else {
                        if (!DDterminal(pnext->p->e[j])) {
                            q = first->next;
                            lastq = first;
                            while (q != nullptr && pnext->p->e[j].p != q->p) {
                                lastq = q;
                                q = q->next;
                            }
                            if (q == nullptr) {
                                q = DDnewListElement();
                                q->p = pnext->p->e[j].p;
                                q->next = nullptr;
                                q->w = n = n + 1;
                                q->cnt = 1;
                                lastq->next = q;
                            } else
                                q->cnt = q->cnt + 1;
                            std::cout << " " << q->w << ":";
                        } else {
                            std::cout << "   T:";
                        }
                        std::cout << " (" << pnext->p->e[j].w << ") ";
                    }
                }
            }

            std::cout << "] " << (intptr_t) pnext->p << "\n";
            i++;
            if (i == limit) {
                std::cout << "Printing terminated after " << limit << " vertices\n";
                return;
            }
            pnext = pnext->next;
        }
    }

    complex_value GetElementOfVector(DDedge e, unsigned long long element) {
        if (DDterminal(e)) {
            complex_value ret;
            ret.r = ret.i = 0;
            return ret;
        }
        complex l;
        l.r = ComplexCache_Avail;
        l.i = l.r->next;
        l.r->val = 1;
        l.i->val = 0;
        do {
            Cmul(l, l, e.w);
            long tmp = (element >> DDinverseOrder[e.p->v]) & 1;
            e = e.p->e[2 * tmp];
        } while (!DDterminal(e));
        Cmul(l, l, e.w);
        complex_value ret;
        ret.r = l.r->val;
        ret.i = l.i->val;

        return ret;
    }


    void DDprintVector(DDedge e) {
        unsigned long long element = 2 << DDinverseOrder[e.p->v];
        for (unsigned long long i = 0; i < element; i++) {
            complex_value amplitude = GetElementOfVector(e, i);
            for (int j = DDinverseOrder[e.p->v]; j >= 0; j--) {
                std::cout << ((i >> j) & 1u);
            }
            std::cout << ": " << amplitude << "\n";

        }
        std::cout << std::flush;
    }


    /*
     * DD2dot export. Nodes representing special matrices (symmetric/identity) are coloured green/red.
     */
    void DD2dot(DDedge e, std::ostream &oss, DDrevlibDescription circ) {
        /* first part of dot file*/
        std::ostringstream nodes;
        /*second part of dot file*/
        std::ostringstream edges;

        edges << "\n";
        /*Initialize Graph*/
        nodes << "digraph \"DD\" {\n"
              << "graph [center=true, ordering=out];\n"
              << "node [shape=circle, center=true];\n"
              << "\"T\" [ shape = box, label=\"1\" ];\n";
        /* Define Nodes */
        ListElementPtr first, q, lastq, pnext;
        first = DDnewListElement();
        first->p = e.p;
        first->next = nullptr;
        first->w = 0;
        first->cnt = 1;

        uint64_t n=0, i=0;

        nodes << "\"R\"";
        //füge Kante zwischen helper node und neuem Knoten hinzu
        if (e.w == COMPLEX_ONE) {
            nodes << " [label=\"\", shape=point];\n";
            edges << "\"R\" -> \"0\"\n";
        } else {
            nodes << " [label=\"\", shape=point];\n";
            edges << "\"R\" -> \"0\" [label=\"(" << e.w << ")\" ];\n";
        }


        pnext = first;
        while (pnext != nullptr) {
            /* Zeichne Knoten*/
            if (pnext->p->ident)
                nodes << "\"" << i << "\" " << "[ label=\""
                      << circ.line[((int) pnext->p->v)].variable
                      << "\" ,style=filled, fillcolor=red ];\n";
            else if (pnext->p->symm)
                nodes << "\"" << i << "\" " << "[ label=\""
                      << circ.line[((int) pnext->p->v)].variable
                      << "\" ,style=filled, fillcolor=green ];\n";
            else
                nodes << "\"" << i << "\" " << "[ label=\""
                      << circ.line[((int) pnext->p->v)].variable
                      << "\" ,style=filled, fillcolor=lightgray ];\n";

            if (pnext->p != DDzero.p) {
                edges << "{rank=same;";
                for (unsigned int k = 0; k < MAXNEDGE; k++) {
                    if (MultMode == 1) {
                        if (k % MAXRADIX != 0) continue;
                    }
                    edges << " \"" << i << "h" << k << "\"";
                }
                edges << "}\n";

                for (int j = 0; j < Nedge; j++) {
                    if (MultMode == 1) {
                        if (j % MAXRADIX != 0) {
                            continue;
                        }
                    }
                    if (pnext->p->e[j].p == nullptr);
                    else {
                        if (!DDterminal(pnext->p->e[j])) {
                            q = first->next;
                            lastq = first;
                            while (q != nullptr && pnext->p->e[j].p != q->p) {
                                lastq = q;
                                q = q->next;
                            }
                            if (q == nullptr) {
                                q = DDnewListElement();
                                q->p = pnext->p->e[j].p;
                                q->next = nullptr;
                                q->w = n = n + 1;
                                q->cnt = 1;
                                lastq->next = q;
                            } else {
                                q->cnt = q->cnt + 1;
                            }
                            nodes << "\"" << i << "h" << j << "\" ";

                            //connect helper node

                            edges << "\"" << i << "\" -> \"" << i << "h" << j << "\" [arrowhead=none";

                            switch (j) {
                                case 0:
                                    edges << ",color=darkgreen";
                                    break;
                                case 1:
                                    edges << ",color=blue";
                                    break;
                                case 2:
                                    edges << ",color=red";
                                    break;
                                case 3:
                                    edges << ",color=gold";
                                    break;
                                default:
                                    break;
                            }
                            edges << "];\n";
                            //füge Kante zwischen helper node und neuem Knoten hinzu
                            if (pnext->p->e[j].w == COMPLEX_ONE) {
                                nodes << " [label=\"\", shape=point];\n";
                                edges << "\"" << i << "h" << j << "\" -> \"" << q->w << "\";\n";
                            } else {
                                nodes << " [label=\"\", shape=point];\n";
                                edges << "\"" << i << "h" << j << "\" -> \"" << q->w
                                      << "\" [label=\" (" << pnext->p->e[j].w << ")\" ];\n";
                            }

                        } else {
                            nodes << "\"" << i << "h" << j << "\" " << " [label=\"\", shape=point ";
                            edges << "\"" << i << "\" -> \"" << i << "h" << j << "\" [arrowhead=none";
                            switch (j) {
                                case 0:
                                    edges << ",color=darkgreen";
                                    break;
                                case 1:
                                    edges << ",color=blue";
                                    break;
                                case 2:
                                    edges << ",color=red";
                                    break;
                                case 3:
                                    edges << ",color=gold";
                                    break;
                                default:
                                    break;
                            }
                            edges << "];\n";
                            //connect helper node
                            if (pnext->p->e[j].w == COMPLEX_ZERO) {
                                nodes << ", fillcolor=red, color=red";
                            } else if (pnext->p->e[j].w == COMPLEX_ONE) {
                                edges << "\"" << i << "h" << j << "\"-> \"T\";\n";
                            } else {
                                edges << "\"" << i << "h" << j << "\"-> \"T\" [label= \"(" << pnext->p->e[j].w << ")\", ];\n";
                            }
                            nodes << "];\n";

                        }
                    }
                }
            }
            i++;
            pnext = pnext->next;
        }
        oss << nodes.str() << edges.str() << "\n}\n" << std::flush;
    }

    // export a DD in .dot format to the file specified by outputFilename
    // and call DOT->SVG export (optional, requires dot package)
    void DDdotExport(DDedge basic, const char *outputFilename, DDrevlibDescription circ, bool show) {
        std::ofstream init(outputFilename);
        DD2dot(basic, init, circ);
        init.close();

        if (show) {
            std::ostringstream oss;
            oss << "dot -Tsvg " << outputFilename << " -o " << outputFilename << ".svg";
            auto str = oss.str(); // required to avoid immediate deallocation of temporary
            std::system(str.c_str());
        }
    }

    void DDdotExportVector(DDedge basic, const char *outputFilename) {
        DDrevlibDescription circ{};

        for (long int i = 0; i <= DDinverseOrder[basic.p->v]; i++) {
            snprintf(circ.line[i].variable, MAXSTRLEN, "x[%ld]", DDinverseOrder[basic.p->v] - i);
        }
        int MultMode_old = MultMode;
        MultMode = 1;

        DDdotExport(basic, outputFilename, circ, true);

        MultMode = MultMode_old;
    }

    void DDdotExportMatrix(DDedge basic, const char *outputFilename) {
        DDrevlibDescription circ{};

        for (long i = 0; i <= DDinverseOrder[basic.p->v]; i++) {
            snprintf(circ.line[i].variable, MAXSTRLEN, "x[%ld]", DDinverseOrder[basic.p->v] - i);
        }
        int MultMode_old = MultMode;
        MultMode = 0;

        DDdotExport(basic, outputFilename, circ, true);

        MultMode = MultMode_old;
    }

    DDedge DDzeroState(int n) {
        DDedge f = DDone;
        DDedge edges[2];
        edges[1]= DDzero;

        for (int p = 0; p < n; p++) {
            edges[0] = f;
            f = DDmakeNonterminal(p, edges, false);
        }
        return f;
    }

    DDedge DDnormalize(DDedge e, bool cached) {
        int max = -1;
        //long double sum = 0.0;
        long double sq = 0.0;
        long double sqmax = 0.0;

        for (int i = 0; i < Nedge; i++) {
            if ((e.p->e[i].p == nullptr || Ceq(e.p->e[i].w, COMPLEX_ZERO))) {
                continue;
            }

            if (max == -1) {
                max = i;
                sqmax = CmagSquared(e.p->e[i].w);
            } else {
                
                sq=CmagSquared(e.p->e[i].w);
                if (sq > sqmax) {
                    max = i;
                    sqmax = sq;
                }
            }
        }
        if (max == -1) {
            if (cached) {
                for (auto & i : e.p->e) {
                    if (i.p != nullptr && i.w != COMPLEX_ZERO) {
                        i.w.i->next = ComplexCache_Avail;
                        ComplexCache_Avail = i.w.r;
                        //cache_num += 2;
                    }
                }
            }
            return DDzero;
        }




        if (cached && e.p->e[max].w != COMPLEX_ONE) {
            e.w = e.p->e[max].w;
        } else {
            complex c;
            c.r = ComplexCache_Avail;
            c.i = ComplexCache_Avail->next;
            c.r->val = CVAL(e.p->e[max].w.r);
            c.i->val = CVAL(e.p->e[max].w.i);
            e.w = Clookup(c);
            if (e.w == COMPLEX_ZERO) {
                return DDzero;
            }
        }

        for (int j = 0; j < Nedge; j++) {
            if (max == j) {
                complex c;
                c.r = ComplexCache_Avail;
                c.i = ComplexCache_Avail->next;
                c.r->val = 1.0;
                c.i->val = 0.0;
                e.p->e[j].w = Clookup(c);
                if (e.p->e[j].w == COMPLEX_ZERO) {
                    e.p->e[j] = DDzero;
                }
            } else if (e.p->e[j].p != nullptr && e.p->e[j].w != COMPLEX_ZERO) {
                if (cached) {
                    e.p->e[j].w.i->next = ComplexCache_Avail;
                    ComplexCache_Avail = e.p->e[j].w.r;
                    //cache_num += 2;
                    Cdiv(e.p->e[j].w, e.p->e[j].w, e.w);
                    e.p->e[j].w = Clookup(e.p->e[j].w);
                    if (e.p->e[j].w == COMPLEX_ZERO) {
                        e.p->e[j] = DDzero;
                    }
                } else {
                    complex c;
                    c.r = ComplexCache_Avail;
                    c.i = ComplexCache_Avail->next;
                    Cdiv(c, e.p->e[j].w, e.w);
                    e.p->e[j].w = Clookup(c);
                    if (e.p->e[j].w == COMPLEX_ZERO) {
                        e.p->e[j] = DDzero;
                    }
                }
            }
        }

        return e;
    }

    //  check if e points to a block, identity, diagonal, symmetric or 0/1-matrix and
    //  marks top node if it does
    

    DDedge DDutLookup(DDedge e) {
    //  lookup a node in the unique table for the appropriate variable - if not found insert it
    //  only normalized nodes shall be stored.
        if (DDterminal(e)) // there is a unique terminal node
        {
            e.p = DDzero.p;
            return e;
        }

        UTlookups++;

        uintptr_t key = 0;
        // note hash function shifts pointer values so that order is important
        // suggested by Dr. Nigel Horspool and helps significantly
        for (unsigned int i = 0; i < Nedge; i++)
            key += ((uintptr_t) (e.p->e[i].p) >> i) + ((uintptr_t) (e.p->e[i].w.r) >> i) +
                   ((uintptr_t) (e.p->e[i].w.i) >> (i + 1));
        key = key & HASHMASK;

        unsigned int v = e.p->v;
        DDnodePtr p = Unique[v][key]; // find pointer to appropriate collision chain
        while (p != nullptr)    // search for a match
        {
            if (memcmp(e.p->e, p->e, Nedge * sizeof(DDedge)) == 0) {
                // Match found
                e.p->next = Avail;    // put node pointed to by e.p on avail chain
                Avail = e.p;

                // NOTE: reference counting is to be adjusted by function invoking the table lookup
                UTmatch++;        // record hash table match

                e.p = p;// and set it to point to node found (with weight unchanged)

                if (p->renormFactor != COMPLEX_ONE) {
                    std::cout << "Debug: table lookup found a node with active renormFactor with v="
                        << p->v << "(id=" << (uintptr_t) p << ").\n";
                    if (p->ref != 0)
                        std::cout << "was active!";
                    else
                        std::cout << "was inactive!";
                    std::exit(1);
                }
                return e;
            }

            UTcol++;        // record hash collision
            p = p->next;
        }
        e.p->next = Unique[v][key]; // if end of chain is reached, this is a new node
        Unique[v][key] = e.p;       // add it to front of collision chain

        DDnodecount++;          // count that it exists
        if (DDnodecount > DDpeaknodecount)
            DDpeaknodecount = DDnodecount;

        //if (!DDterminal(e))
        //    DDcheckSpecialMatrices(e); // check if it is identity or diagonal if nonterminal

        return e;                // and return
    }

    // set compute table to empty and
    // set toffoli gate table to empty and
    // set identity table to empty
    void DDinitComputeTable() {
        for (unsigned int i = 0; i < CTSLOTS; i++) {
            CTable1[i].r = nullptr;
            CTable2[i].r = nullptr;
        }

        for (auto & i : DDid) {
            i.p = nullptr;
        }
        DDnullEdge.p = nullptr;
        DDnullEdge.w = COMPLEX_ZERO;
    }

    void DDgarbageCollect()
// a simple garbage collector that removes nodes with 0 ref count from the unique
// tables placing them on the available space chain
    {
        if (DDnodecount < GCcurrentLimit && ComplexCount < ComplexCurrentLimit)
            return; // do not collect if below GCcurrentLimit node count
        int count = 0;
        int counta = 0;
        for (auto & variable : Unique) {
            for (auto & bucket : variable) {
                DDnodePtr lastp = nullptr;
                DDnodePtr p = bucket;
                while (p != nullptr) {
                    if (p->ref == 0) {
                        if (p == DDterminalNode)
                            std::cerr << "error in garbage collector\n";
                        count++;
                        DDnodePtr nextp = p->next;
                        if (lastp == nullptr)
                            bucket = p->next;
                        else
                            lastp->next = p->next;
                        p->next = Avail;
                        Avail = p;
                        p = nextp;
                    } else {
                        lastp = p;
                        p = p->next;
                        counta++;
                    }
                }
            }
        }
        GCcurrentLimit += GCLIMIT_INC;
        DDnodecount = counta;
        garbageCollectComplexTable();
        DDinitComputeTable(); // IMPORTANT sets compute table to empty after garbage collection
    }

    // get memory space for a node
    DDnodePtr DDgetNode() {
        DDnodePtr r;

        if (Avail != nullptr)    // get node from avail chain if possible
        {
            r = Avail;
            Avail = Avail->next;
        } else {            // otherwise allocate 2000 new nodes
            r = new DDnode[2000];
            DDnodePtr r2 = r+1;
            Avail = r2;
            for (int i = 0; i < 1998; i++, r2++) {
                r2->next = r2+1;
            }
            r2->next = nullptr;
        }
        r->next = nullptr;
        r->ref = 0;            // set reference count to 0
        r->ident = r->diag = r->block = 0;        // mark as not identity or diagonal
        return r;
    }

    // increment reference counter for node e points to
    // and recursively increment reference counter for
    // each child if this is the first reference
    //
    // a ref count saturates and remains unchanged if it has reached
    // MAXREFCNT
    void DDincRef(DDedge e) {
        complexIncRef(e.w);
        if (DDterminal(e))
            return;

        if (e.p->ref == MAXREFCNT) {
            std::cout << "MAXREFCNT reached\n\n\ne.w=" << e.w << "\n";
            DDdebugnode(e.p);
            return;
        }
        e.p->ref++;

        if (e.p->ref == 1) {
            if (!DDterminal(e))
                for (int i = 0; i < Nedge; i++)
                    if (e.p->e[i].p != nullptr) {
                        DDincRef(e.p->e[i]);
                    }
            Active[e.p->v]++;
            ActiveNodeCount++;

            /******* Part added for sifting purposes ********/
            if (e.p->block)
                blockMatrixCounter++;
            /******* by Niemann, November 2012 ********/

        }
    }

    // decrement reference counter for node e points to
    // and recursively decrement reference counter for
    // each child if this is the last reference
    //
    // a ref count saturates and remains unchanged if it has reached
    // MAXREFCNT
    void DDdecRef(DDedge e) {
        complexDecRef(e.w);

        if (DDterminal(e))
            return;

        if (e.p->ref == MAXREFCNT)
            return;


        if (e.p->ref == 0) // ERROR CHECK
        {
            std::cerr <<"error in decref " << e.p->ref << "n";
            DDdebugnode(e.p);
            std::exit(8);
        }
        e.p->ref--;

        if (e.p->ref == 0) {
            if (!DDterminal(e)) {
                for (auto & i : e.p->e) {
                    if (i.p != nullptr) {
                        DDdecRef(i);
                    }
                }
            }
            Active[e.p->v]--;
            if (Active[e.p->v] < 0) {
                std::cerr << "ERROR in decref\n";
                std::exit(1);
            }
            ActiveNodeCount--;

            /******* Part added for sifting purposes ********/
            if (e.p->renormFactor != COMPLEX_ONE) {
                RenormalizationNodeCount--;
                e.p->renormFactor = COMPLEX_ONE;
            }
            if (e.p->block)
                blockMatrixCounter--;
            /******* by Niemann, November 2012 ********/
        }
    }

    // counting number of unique nodes in a QMDD
    unsigned int DDnodeCount(const DDedge e, std::unordered_set<DDnodePtr>& visited) {
        visited.insert(e.p);

        unsigned int sum = 1;
        if (!DDterminal(e)) {
            for (const auto & edge : e.p->e) {
                if (edge.p != nullptr && !visited.count(edge.p)) {
                    sum += DDnodeCount(edge, visited);
                }
            }
        }
        return sum;
    }

    // counts number of unique nodes in a QMDD
    unsigned int DDsize(const DDedge e) {
        std::unordered_set<DDnodePtr> visited(NODECOUNT_BUCKETS); // 2e6
        visited.max_load_factor(10);
        visited.clear();
        return DDnodeCount(e, visited);
    }

    void DDradixPrint(int p, int n)
    // prints p as an n bit Radix number
    // with leading 0's and no CR
    {
        int buffer[MAXN];
        for (int i = 0; i < n; i++) {
            buffer[i] = p % Radix;
            p = p / Radix;
        }
        for (int i = n - 1; i >= 0; i--)
            std::cout << buffer[i];
    }

    inline unsigned long CThash(const DDedge a, const DDedge b) {
        const uintptr_t node_pointer = ((uintptr_t)a.p+(uintptr_t)b.p)>>3u;
        //const uintptr_t weights = (uintptr_t)a.w.i+(uintptr_t)a.w.r+(uintptr_t)b.w.i+(uintptr_t)b.w.r;
        return (node_pointer) & CTMASK;
    }

    inline unsigned long CThash2(DDnodePtr a, const complex_value aw, DDnodePtr b, const complex_value bw) {
        const uintptr_t node_pointer = ((uintptr_t)a+(uintptr_t)b)>>3u;
        const uintptr_t weights = (uintptr_t)(aw.r*1000)+(uintptr_t)(aw.i*2000)+(uintptr_t)(bw.r*3000)+(uintptr_t)(bw.i*4000);
        return (node_pointer+weights) & CTMASK;
    }


    DDedge CTlookup(DDedge a, DDedge b) {
    // Lookup a computation in the compute table
    // return NULL if not a match else returns result of prior computation
        DDedge r;
        r.p = nullptr;
        CTlook[ad]++;

        complex_value aw;
        aw.r = a.w.r->val;
        aw.i = a.w.i->val;

        complex_value bw;
        bw.r = b.w.r->val;
        bw.i = b.w.i->val;

        const unsigned long i = CThash2(a.p, aw, b.p, bw);

        //if (CTable1[i].a == a.p && CTable1[i].b == b.p) {
        //    
        //    if ((CTable1[i].aw.r * bw.r - CTable1[i].aw.i * bw.i) - (CTable1[i].bw.r * aw.r - CTable1[i].bw.i * aw.i)> COMPLEX_TOLERANCE) {
        //        return r;
        //    }
        //    else if ((CTable1[i].aw.r * bw.i + CTable1[i].aw.i * bw.r) - (CTable1[i].bw.r * aw.i + CTable1[i].bw.i * aw.r)> COMPLEX_TOLERANCE) {
        //        return r;
        //    }
        //    else {
        //        float factor = (aw.r+aw.i)/(CTable1[i].aw.r+ CTable1[i].aw.i);

        //        CThit[ad]++;
        //        r.p = CTable1[i].r;

        //        complex c;
        //        c.r = ComplexCache_Avail;
        //        c.i = ComplexCache_Avail->next;
        //        c.r->val = CTable1[i].rw.r*factor;
        //        c.i->val = CTable1[i].rw.i*factor;

        //        if (Ceq(c, COMPLEX_ZERO)) {
        //            return DDzero;
        //             }
        //        else {
        //            ComplexCache_Avail = c.i->next;
        //            //cache_num -= 2;
        //             r.w = c;
        //         }
        //        return r;
        //    }
        //}

        //if (CTable1[i].a == b.p && CTable1[i].b == a.p) {

        //    if ( (CTable1[i].aw.r * aw.r - CTable1[i].aw.i * aw.i)- (CTable1[i].bw.r * bw.r - CTable1[i].bw.i * bw.i)> COMPLEX_TOLERANCE) {
        //        return r;
        //    }
        //    else if ((CTable1[i].aw.r * aw.i + CTable1[i].aw.i * aw.r) - (CTable1[i].bw.r * bw.i + CTable1[i].bw.i * bw.r)> COMPLEX_TOLERANCE) {
        //        return r;
        //    }
        //    else {
        //        float factor = (aw.r + aw.i) / (CTable1[i].bw.r + CTable1[i].bw.i);

        //        CThit[ad]++;
        //        r.p = CTable1[i].r;

        //        complex c;
        //        c.r = ComplexCache_Avail;
        //        c.i = ComplexCache_Avail->next;
        //        c.r->val = CTable1[i].rw.r * factor;
        //        c.i->val = CTable1[i].rw.i * factor;

        //        if (Ceq(c, COMPLEX_ZERO)) {
        //            return DDzero;
        //        }
        //        else {
        //            ComplexCache_Avail = c.i->next;
        //            //cache_num -= 2;
        //            r.w = c;
        //        }
        //        return r;
        //    }
        //}


        if (CTable1[i].a == a.p && Ceq(CTable1[i].aw, aw) && CTable1[i].b == b.p && Ceq(CTable1[i].bw, bw)) {

            CThit[ad]++;
            r.p = CTable1[i].r;

            complex c;
            c.r = ComplexCache_Avail;
            c.i = ComplexCache_Avail->next;
            c.r->val = CTable1[i].rw.r;
            c.i->val = CTable1[i].rw.i;

            if (Ceq(c, COMPLEX_ZERO)) {
                return DDzero;
            }
            else {
                ComplexCache_Avail = c.i->next;
                //cache_num -= 2;
                r.w = c;
            }

            return r;
        
        }

        //if (CTable1[i].a == b.p && Ceq(CTable1[i].aw, bw) && CTable1[i].b == a.p && Ceq(CTable1[i].bw, aw)) {
        //    CThit[ad]++;
        //    r.p = CTable1[i].r;

        //    complex c;
        //    c.r = ComplexCache_Avail;
        //    c.i = ComplexCache_Avail->next;
        //    c.r->val = CTable1[i].rw.r;
        //    c.i->val = CTable1[i].rw.i;

        //    if (Ceq(c, COMPLEX_ZERO)) {
        //        return DDzero;
        //    }
        //    else {
        //        ComplexCache_Avail = c.i->next;
        //        //cache_num -= 2;
        //        r.w = c;
        //    }

        //    return r;

        //}

        return r;
        
    }

    // put an entry into the compute table
    void CTinsert(DDedge a, DDedge b, DDedge r) {

            complex_value aw, bw;
            aw.r = a.w.r->val;
            aw.i = a.w.i->val;
            bw.r = b.w.r->val;
            bw.i = b.w.i->val;

            //std::cout << aw << "   " << bw << std::endl;

            const unsigned long i = CThash2(a.p, aw, b.p, bw);

            CTable1[i].a = a.p;
            CTable1[i].aw = aw;
            CTable1[i].b = b.p;
            CTable1[i].bw = bw;
            CTable1[i].r = r.p;
            CTable1[i].rw.r = r.w.r->val;
            CTable1[i].rw.i = r.w.i->val;

    }


    DDedge CTlookup2(DDedge a, DDedge b, std::vector<float>* key_2_new_key1, std::vector<float>* key_2_new_key2)
    {
        DDedge r;
        r.p = nullptr;
        CTlook[con]++;


        const unsigned long i = CThash(a, b);


        if (CTable2[i].a.p == a.p && CTable2[i].b.p == b.p) {

            std::vector<float> temp_key_2_new_key1;
            temp_key_2_new_key1.assign(key_2_new_key1->begin(), key_2_new_key1->end());
            if (CTable2[i].key_2_new_key1 != temp_key_2_new_key1) return r;
            std::vector<float> temp_key_2_new_key2;
            temp_key_2_new_key2.assign(key_2_new_key2->begin(), key_2_new_key2->end());
            if (CTable2[i].key_2_new_key2 != temp_key_2_new_key2) return r;
            CThit[con]++;
            r.p = CTable2[i].r;
            complex c;
            c.r = ComplexCache_Avail;
            c.i = ComplexCache_Avail->next;
            c.r->val = CTable2[i].rw.r;
            c.i->val = CTable2[i].rw.i;

            if (Ceq(c, COMPLEX_ZERO)) {
                return DDzero;
            }
            else {
                ComplexCache_Avail = c.i->next;
                //cache_num -= 2;
                r.w = c;
            }

            return r;
        }
        return r;
    }

    void CTinsert2(DDedge a, DDedge b, DDedge r, std::vector<float>* key_2_new_key1, std::vector<float>* key_2_new_key2)
    {
        
        const unsigned long i = CThash(a, b);

        CTable2[i].a = a;
        CTable2[i].b = b;
        CTable2[i].key_2_new_key1.assign(key_2_new_key1->begin(), key_2_new_key1->end());
        CTable2[i].key_2_new_key2.assign(key_2_new_key2->begin(), key_2_new_key2->end());
        CTable2[i].r = r.p;
        CTable2[i].rw.r = r.w.r->val;
        CTable2[i].rw.i = r.w.i->val;
    }




    // recursively scan an QMDD putting values in entries of mat
    // v is the variable index
    void DDfillmat(complex mat[MAXDIM][MAXDIM], DDedge a, int r, int c, int dim, short v, const char vtype[]) {
        if (a.p == nullptr) {
            return;
        }

        if (v == -1) { // terminal node case
            if (r >= MAXDIM || c >= MAXDIM) {
                std::cerr << "out of bounds, r=" << r << ", c=" << c << "\n";
                return;
            }
            complex co;
            co.r = ComplexCache_Avail;
            co.i = ComplexCache_Avail->next;
            ComplexCache_Avail = co.i->next;

            co.r->val = CVAL(a.w.r);
            co.i->val = CVAL(a.w.i);

            mat[r][c] = co;
        } else {
            bool expand = (DDterminal(a)) || v != DDinverseOrder[a.p->v];
            for (int i = 0; i < Nedge; i++) {
                if (expand) {
                    DDfillmat(mat, a, r + (i / Radix) * dim / Radix,
                              c + (i % Radix) * dim / Radix, dim / Radix, v - 1,
                              vtype);
                } else {
                    DDedge e = a.p->e[i];

                    complex co;
                    co.r = ComplexCache_Avail;
                    co.i = ComplexCache_Avail->next;
                    ComplexCache_Avail = co.i->next;

                    Cmul(co, a.w, e.w);
                    e.w = co;

                    DDfillmat(mat, e, r + (i / Radix) * dim / Radix,c + (i % Radix) * dim / Radix,
                            dim / Radix, v - 1, vtype);

                    co.i->next = ComplexCache_Avail;
                    ComplexCache_Avail = co.r;
                }
            }
        }
    }

    void DDpermPrint(DDedge e, int row, int col) {
        if (DDterminal(e)) {
            if (e.w != COMPLEX_ONE)
                std::cerr << "error in permutation printing\n";
            else
                PermList[col] = row;
        } else
            for (int i = 0; i < Nedge; i++)
                if (e.p->e[i].p != nullptr && e.p->e[i].w != COMPLEX_ZERO)
                    DDpermPrint(e.p->e[i], row * Radix + i / Radix,col * Radix + i % Radix);
    }

/***************************************

 Public Routines

 ***************************************/

    // make a DD nonterminal node and return an edge pointing to it
    // node is not recreated if it already exists
    DDedge DDmakeNonterminal(short v, DDedge edge[MAXNEDGE], bool cached) {

        //std::cout <<"509  " << e.p->v << "   " << e.p->e[0].p->v << std::endl;

        if (edge[0].p == edge[1].p && Ceq(edge[0].w, edge[1].w)) {

            //std::cout << "513  " << e.p->v << e.p->e[0].p->v << std::endl;

            if (cached) {
                if (edge[1].w != COMPLEX_ZERO) {
                    edge[1].w.i->next = ComplexCache_Avail;
                    ComplexCache_Avail = edge[1].w.r;
                    //cache_num += 2;
                    return edge[0];
                }
                //if (e.p->e[0].p != nullptr && e.p->e[0].w != COMPLEX_ZERO) {
                //    e.p->e[0].w.i->next = ComplexCache_Avail;
                //    ComplexCache_Avail = e.p->e[0].w.r;
                //    cache_num += 2;

                //}
                return edge[0];

            }
            return edge[0];
        }


        DDedge e;
        e.p = DDgetNode();  // get space and form node
        e.w = COMPLEX_ONE;
        e.p->v = v;
        e.p->renormFactor = COMPLEX_ONE;
        //e.p->computeSpecialMatricesFlag = globalComputeSpecialMatricesFlag;

        memcpy(e.p->e, edge, Nedge * sizeof(DDedge));
        e = DDnormalize(e, cached); // normalize it
        e = DDutLookup(e);  // look it up in the unique tables
        return e;          // return result
    }

    // make a terminal - actually make an edge with appropriate weight
    // as there is only complex_one terminal DDone
    DDedge DDmakeTerminal(complex w) {
        DDedge e;
        e.p = DDterminalNode;
        e.w = w;
        return e;
    }
    // initialize DD package - must be called before other routines are used
    void DDinit(const bool verbose) {
        if (verbose) {
            std::cout << DDversion
                << "\n  compiled: " << __DATE__ << " " << __TIME__
                << "\n  edge size: " << sizeof(DDedge) << " bytes"
                << "\n  node size: " << sizeof(DDnode) << " bytes (with edges: " << sizeof(DDnode) + MAXNEDGE * sizeof(DDedge) << " bytes)"
                << "\n  max variables: " << MAXN
                << "\n  UniqueTable buckets: " << NBUCKET
                << "\n  ComputeTable slots: " << CTSLOTS
                << "\n  ToffoliTable slots: " << TTSLOTS
                << "\n  garbage collection limit: " << GCLIMIT1
                << "\n  garbage collection increment: " << GCLIMIT_INC
                << "\n" << std::flush;
        }

        complexInit();       // init complex number package
        DDinitComputeTable();  // init computed table to empty

        GCcurrentLimit = GCLIMIT1; // set initial garbage collection limit

        ComplexCurrentLimit = 100000;

        UTcol = UTmatch = UTlookups = 0;

        DDnodecount = 0;            // zero node counter
        DDpeaknodecount = 0;
        Nlabel = 0;                        // zero variable label counter
        Nop[0] = Nop[1] = Nop[2] = 0;        // zero op counter
        CTlook[0] = CTlook[1] = CTlook[2] = CThit[0] = CThit[1] = CThit[2] = 0;    // zero CTable counters
        Avail = nullptr;                // set available node list to empty
        Lavail = nullptr;                // set available element list to empty
        DDterminalNode = DDgetNode();// create terminal node - note does not go in unique table
        DDterminalNode->ident = 1;
        DDterminalNode->diag = 1;
        DDterminalNode->block = 0;
        DDterminalNode->symm = 1;
        DDterminalNode->c01 = 1;
        DDterminalNode->renormFactor = COMPLEX_ONE;
        DDterminalNode->computeSpecialMatricesFlag = 0;
        for (auto & i : DDterminalNode->e) {
            i.p = nullptr;
            i.w = COMPLEX_ZERO;
        }
        DDterminalNode->v = -1;

        DDzero = DDmakeTerminal(COMPLEX_ZERO);
        DDone = DDmakeTerminal(COMPLEX_ONE);


        for (auto & variable : Unique) {
            for (auto & bucket : variable) {
                bucket = nullptr; // set unique tables to empty
            }
        }

        ActiveNodeCount = 0;
        if (verbose) {
            std::cout << "DD initialization complete\n----------------------------------------------------------\n";
        }
    }

    // adds two matrices represented by DD
    // the two DD should have the same variable set and ordering
    DDedge T_add2(DDedge x, DDedge y) {
        
        if (to_test) {
            std::cout << "add  " << x.p->v << "," << y.p->v << " " << x.w << " " << y.w << " " << x.p << " " << y.p << std::endl;
        }
        if (x.p == nullptr) {
            return y;  // handles partial matrices i.e.
        }
        if (y.p == nullptr) {
            return x;  // column and row vetors
        }
        Nop[ad]++;

        if (x.w == COMPLEX_ZERO) {
            if (y.w == COMPLEX_ZERO) {
                return y;
            }
            complex c;
            c.r = ComplexCache_Avail;
            c.i = ComplexCache_Avail->next;
            ComplexCache_Avail = c.i->next;
            //cache_num -= 2;
            c.r->val = y.w.r->val;
            c.i->val = y.w.i->val;
            y.w = c;

            return y;
        }
        if (y.w == COMPLEX_ZERO) {
            complex c;
            c.r = ComplexCache_Avail;
            c.i = ComplexCache_Avail->next;
            ComplexCache_Avail = c.i->next;
            //cache_num -= 2;
            c.r->val = x.w.r->val;
            c.i->val = x.w.i->val;
            x.w = c;

            return x;
        }
        if (x.p == y.p) {
            DDedge r = y;

            complex result;
            result.r = ComplexCache_Avail;
            result.i = result.r->next;

            Cadd(result, x.w, y.w);
            if (Ceq(result, COMPLEX_ZERO)) {
                return DDzero;
            }
            ComplexCache_Avail = result.i->next;
            //cache_num -= 2;
            r.w = result;

            return r;
        }
        //uintptr_t

        if (x.p > y.p) {
        
            return T_add2(y, x);
        }

        const complex xweight=x.w;
        const complex yweight = y.w;
        complex temp;
        if (xweight != COMPLEX_ONE) {
            x.w = COMPLEX_ONE;
            temp.r = ComplexCache_Avail;
            temp.i = temp.r->next;
            Cdiv(temp, y.w, xweight);
            y.w = temp;
            ComplexCache_Avail = temp.i->next;
        }



        DDedge r = CTlookup(x, y);
        if (r.p != nullptr) {
            if (xweight != COMPLEX_ONE) {
                x.w = xweight;
                y.w = yweight;
                temp.i->next = ComplexCache_Avail;
                ComplexCache_Avail = temp.r;
            }
            if (r.w != COMPLEX_ZERO) {
                Cmul(r.w, r.w, xweight);
            }
            return (r);
        }

        int w;
        if (DDterminal(x)) {
            w = y.p->v;
        } else {
            w = x.p->v;
            if (!DDterminal(y) && y.p->v > w) {
                w = y.p->v;
            }
        }

        DDedge e1, e2, e[MAXNEDGE];
        for (int i = 0; i < Nedge; i++) {
            if (!DDterminal(x) && x.p->v == w) {
                e1 = x.p->e[i];

                if (e1.w != COMPLEX_ZERO) {
                    complex c;
                    c.r = ComplexCache_Avail;
                    c.i = ComplexCache_Avail->next;
                    ComplexCache_Avail = c.i->next;
                    //cache_num -= 2;
                    Cmul(c, e1.w, x.w);
                    e1.w = c;
                }
            } else {
                    e1 = x;
                    if (y.p->e[i].p == nullptr) {
                        e1 = DDnullEdge;
                    }
            }
            if (!DDterminal(y) && y.p->v == w) {
                e2 = y.p->e[i];

                if (e2.w != COMPLEX_ZERO) {
                    complex c;
                    c.r = ComplexCache_Avail;
                    c.i = ComplexCache_Avail->next;
                    ComplexCache_Avail = c.i->next;
                    
                    //cache_num -= 2;

                    Cmul(c, e2.w, y.w);
                    e2.w = c;
                }
            } else {
                    e2 = y;
                    if (x.p->e[i].p == nullptr) {
                        e2 = DDnullEdge;
                    }

            }

            e[i] = T_add2(e1, e2);

            if (!DDterminal(x) && x.p->v == w && e1.w != COMPLEX_ZERO) {
                e1.w.i->next = ComplexCache_Avail;
                ComplexCache_Avail = e1.w.r;
                //cache_num += 2;
            }

            if (!DDterminal(y) && y.p->v == w && e2.w != COMPLEX_ZERO) {
                e2.w.i->next = ComplexCache_Avail;
                ComplexCache_Avail = e2.w.r;
                //cache_num += 2;
            }
        }

        r = DDmakeNonterminal(w, e, true);

        CTinsert(x, y, r);
        if (xweight != COMPLEX_ONE) {
            x.w = xweight;
            y.w = yweight;
            temp.i->next = ComplexCache_Avail;
            ComplexCache_Avail = temp.r;
        }
        if (r.w != COMPLEX_ZERO) {
            Cmul(r.w, r.w, xweight);
        }
        return r;
    }

    DDedge T_add(DDedge x, DDedge y) {
        DDedge result = T_add2(x, y);

        if (result.w != COMPLEX_ZERO) {
            complex c = Clookup(result.w);
            result.w.i->next = ComplexCache_Avail;
            ComplexCache_Avail = result.w.r;
            result.w = c;
        }
        return result;
    }


    DDedge cont2(DDedge x, DDedge y, std::vector<float>* key_2_new_key1, std::vector<float>* key_2_new_key2, int var_num) {
        
        if (to_test) {
            std::cout << "cont" << x.p->v << "," << y.p->v << std::endl;
        }

        if (x.p == nullptr)
            return x;
        if (y.p == nullptr)
            return y;
        Nop[con]++;

        if (x.w == COMPLEX_ZERO || y.w == COMPLEX_ZERO) {
            return DDzero;
        }

        if (x.p->v==-1 && y.p->v==-1)
        {
            complex result;
            result.r = ComplexCache_Avail;
            result.i = result.r->next;
            ComplexCache_Avail = result.i->next;
            //cache_num -= 2;
            Cmul(result, x.w, y.w);
            if (var_num > 0) {
                complex temp;
                temp.r = ComplexCache_Avail;
                temp.i = temp.r->next;
                temp.r->val = pow(2, var_num);
                temp.i->val = 0;
                Cmul(result, result, temp);
            }
            return DDmakeTerminal(result);
        }


        if (x.p->v == -1 && var_num == 0 && int(key_2_new_key2->at(y.p->v + 1)) == y.p->v) {
            complex result;
            result.r = ComplexCache_Avail;
            result.i = result.r->next;
            ComplexCache_Avail = result.i->next;
            //cache_num -= 2;
            Cmul(result, x.w, y.w);
            DDedge res = { y.p ,result};
            return res;
        }

        if (y.p->v == -1 && var_num == 0 && int(key_2_new_key1->at(x.p->v + 1)) == x.p->v) {
            complex result;
            result.r = ComplexCache_Avail;
            result.i = result.r->next;
            ComplexCache_Avail = result.i->next;
            //cache_num -= 2;
            Cmul(result, x.w, y.w);
            DDedge res = { x.p ,result };
            return res;
        }

        const complex xweight = x.w;
        const complex yweight = y.w;
        x.w = COMPLEX_ONE;
        y.w = COMPLEX_ONE;

        int k1 = x.p->v + 1;
        int k2 = y.p->v + 1;


        std::vector<float> key_2_new_key1_t(key_2_new_key1->begin(), key_2_new_key1->begin() + k1 + 1);
        std::vector<float> key_2_new_key2_t(key_2_new_key2->begin(), key_2_new_key2->begin() + k2 + 1);


        DDedge r = CTlookup2(x, y, &key_2_new_key1_t, &key_2_new_key2_t);
        if (r.p != nullptr) {
            x.w = xweight;
            y.w = yweight;
            if (r.w != COMPLEX_ZERO) {
                Cmul(r.w, r.w, xweight);
                Cmul(r.w, r.w, yweight);
            }
            return r;
        }

        
        float newk1 = key_2_new_key1->at(k1);

        float newk2 = key_2_new_key2->at(k2);

        DDedge e1{}, e2{}, e[Nedge];
        if (newk1 > newk2) {
            if (int(newk1 * 2) % 2 == 1) {
                for (int k = 0; k < 2; ++k) {
                    e1 = x.p->e[k];
                    e2 = y;
                    e[k] = cont2(e1, e2, &key_2_new_key1_t, key_2_new_key2, var_num - 1);
                }
                if (e[0].w == COMPLEX_ZERO && e[1].w == COMPLEX_ZERO) {
                    r = DDzero;
                }
                else if (e[0].w == COMPLEX_ZERO) {
                    r = e[1];
                }
                else if (e[1].w == COMPLEX_ZERO) {
                    r = e[0];
                }
                else {
                    r = T_add2(e[0], e[1]);
                    e[0].w.i->next = ComplexCache_Avail;
                    ComplexCache_Avail = e[0].w.r;
                    e[1].w.i->next = ComplexCache_Avail;
                    ComplexCache_Avail = e[1].w.r;
                    //cache_num += 4;
                }
            }
            else {
                for (int k = 0; k < 2; ++k) {
                    e1 = x.p->e[k];
                    e2 = y;
                    e[k] = cont2(e1, e2, &key_2_new_key1_t, key_2_new_key2, var_num);
                }
                r = DDmakeNonterminal(short(newk1), e, true);
            }
        }
        else if (newk1 < newk2) {

            if (int(newk2 * 2) % 2 == 1) {
                for (int k = 0; k < 2; ++k) {
                    e1 = x;
                    e2 = y.p->e[k];
                    e[k] = cont2(e1, e2, key_2_new_key1, &key_2_new_key2_t, var_num - 1);
                }
                if (e[0].w == COMPLEX_ZERO && e[1].w == COMPLEX_ZERO) {
                    r = DDzero;
                }
                else if (e[0].w == COMPLEX_ZERO) {
                    r = e[1];
                }
                else if (e[1].w == COMPLEX_ZERO) {
                    r = e[0];
                }
                else {
                    r = T_add2(e[0], e[1]);
                    e[0].w.i->next = ComplexCache_Avail;
                    ComplexCache_Avail = e[0].w.r;
                    e[1].w.i->next = ComplexCache_Avail;
                    ComplexCache_Avail = e[1].w.r;
                    //cache_num += 4;
                }
            }
            else {
                for (int k = 0; k < 2; ++k) {
                    e1 = x;
                    e2 = y.p->e[k];
                    e[k] = cont2(e1, e2, key_2_new_key1, &key_2_new_key2_t, var_num);
                }
                r = DDmakeNonterminal(short(newk2), e, true);
            }

        }
        else {
            if (int(newk2 * 2) % 2 == 1) {
                for (int k = 0; k < 2; ++k) {
                    e1 = x.p->e[k];
                    e2 = y.p->e[k];
                    e[k] = cont2(e1, e2, &key_2_new_key1_t, &key_2_new_key2_t, var_num - 1);
                }
                if (e[0].w == COMPLEX_ZERO && e[1].w == COMPLEX_ZERO) {
                    r = DDzero;
                }
                else if (e[0].w == COMPLEX_ZERO) {
                    r = e[1];
                }
                else if (e[1].w == COMPLEX_ZERO) {
                    r = e[0];
                }
                else {
                    r = T_add2(e[0], e[1]);
                    e[0].w.i->next = ComplexCache_Avail;
                    ComplexCache_Avail = e[0].w.r;
                    e[1].w.i->next = ComplexCache_Avail;
                    ComplexCache_Avail = e[1].w.r;
                    //cache_num += 4;
                }
            }
            else {
                for (int k = 0; k < 2; ++k) {
                    e1 = x.p->e[k];
                    e2 = y.p->e[k];
                    e[k] = cont2(e1, e2, &key_2_new_key1_t, &key_2_new_key2_t, var_num);
                }
                r = DDmakeNonterminal(short(newk1), e, true);
            }
        }

        CTinsert2(x, y, r, &key_2_new_key1_t, &key_2_new_key2_t);
        x.w = xweight;
        y.w = yweight;

        if (r.w != COMPLEX_ZERO) {
            Cmul(r.w, r.w, xweight);
            Cmul(r.w, r.w, yweight);

        }
        return r;
    }


    TDD cont(TDD tdd1, TDD tdd2) {

        std::vector<Index> var_out;
        std::vector<std::string> var_cont_temp;
        std::vector<std::string> var_cont;
        std::vector<std::string> var_out_key;

        int k;
        int k1;

        for (k = 0; k < tdd1.index_set.size(); ++k) {
            bool flag = true;
            for (k1 = 0; k1 < tdd2.index_set.size(); ++k1) {
                if (tdd2.index_set[k1].idx == tdd1.index_set[k].idx && tdd2.index_set[k1].key == tdd1.index_set[k].key) {
                    var_cont_temp.push_back(tdd1.index_set[k].key);
                    flag = false;
                    break;
                }
            }

            if (flag) {
                var_out.push_back(tdd1.index_set[k]);
                var_out_key.push_back(tdd1.index_set[k].key);
            }
        }
        for (k = 0; k < tdd2.index_set.size(); ++k) {
            bool flag = true;
            for (k1 = 0; k1 < tdd1.index_set.size(); ++k1) {
                if (tdd1.index_set[k1].idx == tdd2.index_set[k].idx && tdd1.index_set[k1].key == tdd2.index_set[k].key) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                var_out.push_back(tdd2.index_set[k]);
                var_out_key.push_back(tdd2.index_set[k].key);
            }
        }

        for (k = 0; k < var_cont_temp.size(); ++k) {
            if (find(var_out_key.begin(), var_out_key.end(), var_cont_temp[k]) == var_out_key.end()) {
                if (find(var_cont.begin(), var_cont.end(), var_cont_temp[k]) == var_cont.end()) {
                    var_cont.push_back(var_cont_temp[k]);

                }
            }
        }


        std::vector<float> key_2_new_key1 = { -1 };
        std::vector<float> key_2_new_key2 = { -1 };
        std::vector<std::string> new_key_2_index;
        k1 = 0;
        int k2 = 0;
        int new_key = 0;
        int m1 = tdd1.key_2_index.size();
        int m2 = tdd2.key_2_index.size();
        if (to_test) {
            std::cout << "TDD1: ";
            for (const auto& element : tdd1.key_2_index) {
                std::cout << element << " ";
            }
            std::cout << std::endl;

            std::cout << "TDD2: ";
            for (const auto& element : tdd2.key_2_index) {
                std::cout << element << " ";
            }
            std::cout << std::endl;
        }


        //std::cout << m1 << "  " << m2<<std::endl;

        while (k1 < m1 || k2 < m2) {

            if (k1 == m1) {
                for (k2; k2 < m2; ++k2)
                {
                    key_2_new_key2.push_back(new_key);
                    new_key_2_index.push_back(tdd2.key_2_index[k2]);
                    new_key++;
                }
                break;
            }
            if (k2 == m2) {
                for (k1; k1 < m1; ++k1)
                {
                    key_2_new_key1.push_back(new_key);
                    new_key_2_index.push_back(tdd1.key_2_index[k1]);
                    new_key++;
                }
                break;
            }

            if (varOrder[tdd1.key_2_index[k1]] < varOrder[tdd2.key_2_index[k2]]) {
                key_2_new_key1.push_back(new_key);
                new_key_2_index.push_back(tdd1.key_2_index[k1]);
                new_key++;
                k1++;
            }
            else if (varOrder[tdd1.key_2_index[k1]] > varOrder[tdd2.key_2_index[k2]]) {
                key_2_new_key2.push_back(new_key);
                new_key_2_index.push_back(tdd2.key_2_index[k2]);
                new_key++;
                k2++;
            }
            else if (find(var_out_key.begin(), var_out_key.end(), tdd1.key_2_index[k1]) == var_out_key.end()) {

                key_2_new_key1.push_back(new_key - 0.5);
                key_2_new_key2.push_back(new_key - 0.5);
                k1++;
                k2++;
            }
            else {
                key_2_new_key1.push_back(new_key);
                key_2_new_key2.push_back(new_key);
                new_key_2_index.push_back(tdd1.key_2_index[k1]);
                new_key++;
                k1++;
                k2++;
            }
        }

        TDD res;
        res.index_set = var_out;
        res.key_2_index = new_key_2_index;

        //std::cout << cache_num << std::endl;
        res.e = cont2(tdd1.e, tdd2.e, &key_2_new_key1, &key_2_new_key2, var_cont.size());

        var_out.clear();
        var_cont_temp.clear();
        var_out_key.clear();
        var_cont.clear();

        if (res.e.w != COMPLEX_ZERO) {
            complex c = Clookup(res.e.w);
            res.e.w.i->next = ComplexCache_Avail;
            ComplexCache_Avail = res.e.w.r;
            res.e.w = c;
            //cache_num += 2;
        }
        //std::cout << cache_num << std::endl;
        return res;
    }


    TDD Matrix2TDD(const DD_matrix mat, std::vector<Index> var_out)
    {
        //std::cout << var_out[0].key<<std::endl;
        //std::cout << var_out[1].key << std::endl;
        TDD low, high, res;
        DDedge e_low[Nedge], e_high[Nedge], e[Nedge], e_temp[Radix * Radix];
        complex c;
        c.r = ComplexCache_Avail;
        c.i = ComplexCache_Avail->next;

        for (int i = 0; i < Radix; i++) {
            for (int j = 0; j < Radix; j++) {
                if (mat[i][j].r == 0.0 && mat[i][j].i == 0.0) {
                    e_temp[i * Radix + j] = DDzero;
                }
                else {
                    c.r->val = mat[i][j].r;
                    c.i->val = mat[i][j].i;
                    e_temp[i * Radix + j] = DDmakeTerminal(Clookup(c));
                }
            }
        }


        std::vector<std::string> key_2_index;
        if (varOrder[var_out[0].key] < varOrder[var_out[1].key]) {
            e_low[0] = e_temp[0];
            e_low[1] = e_temp[1];
            e_high[0] = e_temp[2];
            e_high[1] = e_temp[3];
            key_2_index = { var_out[0].key,var_out[1].key };
        }
        else {
            e_low[0] = e_temp[0];
            e_low[1] = e_temp[2];
            e_high[0] = e_temp[1];
            e_high[1] = e_temp[3];
            key_2_index = { var_out[1].key,var_out[0].key };
        }


        if (e_low[0].p==e_low[1].p and e_low[0].w == e_low[1].w) {
            low.e = e_low[0];
        }
        else {
            low.e = DDmakeNonterminal(0, e_low, false);
        }
        if (e_high[0].p == e_high[1].p and e_high[0].w == e_high[1].w) {
            high.e = e_high[0];
        }
        else {
            high.e = DDmakeNonterminal(0, e_high, false);
        }
        if (low.e.p== high.e.p and low.e.w==high.e.w) {
            res.e = low.e;
        }
        else {
            e[0] = low.e;
            e[1] = high.e;
            res.e = DDmakeNonterminal(1, e, false);
        }
        res.index_set = var_out;
        res.key_2_index = key_2_index;
        return res;
    }

    TDD diag_matrix_2_TDD(const DD_matrix mat, std::vector<Index> var_out)
    {

        TDD res;
        DDedge e_temp[Radix];

        complex c;
        c.r = ComplexCache_Avail;
        c.i = ComplexCache_Avail->next;

        for (int i = 0; i < Radix; i++) {

            if (mat[i][i].r == 0.0 && mat[i][i].i == 0.0) {
                e_temp[i] = DDzero;
            }
            else {
                if (mat[i][i].r == 1.0 && mat[i][i].i == 0.0) {
                    e_temp[i] = DDone;
                }
                else {
                    c.r->val = mat[i][i].r;
                    c.i->val = mat[i][i].i;
                    e_temp[i] = DDmakeTerminal(Clookup(c));
                }

            }

        }

        res.e = DDmakeNonterminal(0, e_temp,false);
        res.index_set = var_out;
        res.key_2_index = { var_out[0].key };
        return res;
    }

    TDD cnot_2_TDD(std::vector<Index> var, int ca = 1) {


        TDD low, high, res;
        DDedge e[2];
        if (ca == 1) {
            if (varOrder[var[0].key] > varOrder[var[3].key] && varOrder[var[0].key] > varOrder[var[4].key]) {
                low = Matrix2TDD(Im, { var[3] ,var[4] });
                high = Matrix2TDD(Nm, { var[3] ,var[4] });
                e[0] = low.e;
                e[1] = high.e;
                res.e = DDmakeNonterminal(2, e,false);
                res.index_set = { var[0],var[2],var[3],var[4] };
                low.key_2_index.push_back(var[0].key);
                res.key_2_index = low.key_2_index;
            }
            else if (varOrder[var[3].key] > varOrder[var[0].key] && varOrder[var[3].key] > varOrder[var[4].key]) {
                low = Matrix2TDD(Im, { var[0] ,var[4] });
                high = Matrix2TDD(Nm, { var[0] ,var[4] });
                e[0] = low.e;
                e[1] = high.e;
                res.e = DDmakeNonterminal(2, e,false);
                res.index_set = { var[0],var[2],var[3],var[4] };
                low.key_2_index.push_back(var[3].key);
                res.key_2_index = low.key_2_index;
            }
            else {
                low = Matrix2TDD(Im, { var[0] ,var[3] });
                high = Matrix2TDD(Nm, { var[0] ,var[3] });
                e[0] = low.e;
                e[1] = high.e;
                res.e = DDmakeNonterminal(2, e,false);
                res.index_set = { var[0],var[2],var[3],var[4] };
                low.key_2_index.push_back(var[4].key);
                res.key_2_index = low.key_2_index;
            }
        }
        if (ca == 2) {
            res.e = DDone;
            res.index_set = { var[0],var[1],var[2] };
            res.key_2_index = { var[0].key };
        }
        if (ca == 3) {
            if (varOrder[var[1].key] > varOrder[var[3].key] && varOrder[var[1].key] > varOrder[var[4].key]) {
                low = Matrix2TDD(Im, { var[3] ,var[4] });
                high = Matrix2TDD(Nm, { var[3] ,var[4] });
                e[0] = low.e;
                e[1] = high.e;
                res.e = DDmakeNonterminal(2, e, false);
                res.index_set = { var[1],var[3],var[4] };
                low.key_2_index.push_back(var[1].key);
                res.key_2_index = low.key_2_index;

            }
            else if (varOrder[var[3].key] > varOrder[var[1].key] && varOrder[var[3].key] > varOrder[var[4].key]) {
                low = Matrix2TDD(Im, { var[1] ,var[4] });
                high = Matrix2TDD(Nm, { var[1] ,var[4] });
                e[0] = low.e;
                e[1] = high.e;
                res.e = DDmakeNonterminal(2, e, false);
                res.index_set = { var[1],var[3],var[4] };
                low.key_2_index.push_back(var[3].key);
                res.key_2_index = low.key_2_index;

            }
            else {
                low = Matrix2TDD(Im, { var[1] ,var[3] });
                high = Matrix2TDD(Nm, { var[1] ,var[3] });
                e[0] = low.e;
                e[1] = high.e;
                res.e = DDmakeNonterminal(2, e, false);
                res.index_set = { var[1],var[3],var[4] };
                low.key_2_index.push_back(var[4].key);
                res.key_2_index = low.key_2_index;
            }
        }
        return res;
    }


    //
    // v is the variable index for the top vertex
    void DDmatrixPrint(DDedge a, short v, const char vtype[], std::ostream &os) {
        complex mat[MAXDIM][MAXDIM];
        complex cTabPrint[MAXDIM * MAXDIM + 2];
        int cTabEntries = 0;
        bool cTabPrintFlag = false;
        int n;

        if (DDterminal(a)) {
            n = 0;
        } else {
            n = v + 1;
        }
        int m = 1;
        for (int i = 0; i < n; i++) {
            m *= Radix;
        }

        if (n > MAXND) {
            std::cerr << "Matrix is too big to print. No. of vars=" << n << "\n";
            std::exit(1);
        }

        DDfillmat(mat, a, 0, 0, m, v, vtype); // convert to matrix


        cTabPrint[0] = COMPLEX_ZERO;
        cTabPrint[1] = COMPLEX_ONE;
        cTabEntries = 2;

        for (int i = 0; i < m; i++) {          // display matrix
            for (int j = 0; j < m; j++) {
                int k = 0;
                while (k < cTabEntries && !Ceq(cTabPrint[k], mat[i][j])) {
                    k++;
                }

                if (k == cTabEntries) {
                    cTabEntries++;
                    cTabPrint[k] = mat[i][j];
                }
                cTabPrintFlag = true;
                if (k < 10) {
                    os << " ";
                }
                os << k << " ";
                if (j == m / 2 - 1) {
                    os << "|";
                }
            }
            os << "\n";
            if (i == m / 2 - 1) {
                for (int j = 0; j < m; j++) {
                    os << " --";
                }
                os << "\n";
            }
        }
        if (cTabPrintFlag) {
            os << "ComplexTable values: "; //(0): 0; (1): 1; ";

            for (int i = 0; i < cTabEntries; i++) {
                os << "(" << i << "):" << cTabPrint[i] << "; ";
            }
        }

        os << "\n";

        for (int i = 0; i < (1 << n); i++) {
            for (int j = 0; j < (1 << n); j++) {
                mat[i][j].i->next = ComplexCache_Avail;
                ComplexCache_Avail = mat[i][j].r;
            }
        }
    }



    void DDmatrixPrint(DDedge a, short v, char vtype[]) {
        DDmatrixPrint(a, v, vtype, std::cout);
    }

    void DDmatrixPrint2(DDedge a, std::ostream &os) {
        char v[MAXN];
        int i;

        if (DDterminal(a)) {
            os << a.w << "\n";
        } else {
            for (i = 0; i < MAXN; i++) {
                v[i] = 0;
            }
            DDmatrixPrint(a, a.p->v, v, os);
        }
    }

    void DDmatrixPrint2(DDedge a, std::ostream &os, short n) {
        if (DDterminal(a)) {
            os << a.w << "\n";
        } else {
            char v[MAXN]{};
            DDmatrixPrint(a, n, v, os);
        }
    }

    void DDmatrixPrint2(DDedge a) {
        if (DDterminal(a)) {
            std::cout << a.w << "\n";
        } else {
            char v[MAXN]{};
            DDmatrixPrint(a, DDinverseOrder[a.p->v], v);
        }
    }

    // displays DD package statistics
    void DDstatistics() {
        std::cout << "\nDD statistics:"
            << "\n  Current # nodes in UniqueTable: " << DDnodecount
            << "\n  Total compute table lookups: " << CTlook[0] + CTlook[1] + CTlook[2]
            << "\n  Number of operations:"
            << "\n    add:  " <<  Nop[ad]
            << "\n    mult: " <<  Nop[con]
            << "\n  Compute table hit ratios (hits/looks/ratio):"
            << "\n    adds: " << CThit[ad] << " / " << CTlook[ad] << " / " << (double) CThit[ad] / (double)CTlook[ad]
            << "\n    mult: " << CThit[con] << " / " << CTlook[con] << " / " << (double) CThit[con] / (double)CTlook[con]
            << "\n  UniqueTable:"
            << "\n    Collisions: " << UTcol
            << "\n    Matches:    " << UTmatch
            << "\n" << std::flush;
    }

    // print number of active nodes for variables 0 to n-1
    void DDprintActive(int n) {
        std::cout << "#printActive: " << ActiveNodeCount << ". ";
        for (int i = 0; i < n; i++) {
            std::cout << " " << Active[i] << " ";
        }
        std::cout << "\n";
    }

  } 