/*
 * This file is part of IIC-JKU DD package which is released under the MIT license.
 * See file README.md or go to http://iic.jku.at/eda/research/quantum_dd/ for more information.
 */

#include "TDDpackage.h"
#include "algorithm"
#include<math.h>
#include<math.h>
#include <map>
#include <string>
namespace dd {

	Node Package::terminal{ nullptr, {{ nullptr, CN::ZERO}, { nullptr, CN::ZERO }}, 0, std::to_string(-1), true, true};
	constexpr Edge Package::DDzero;
	constexpr Edge Package::DDone;


    ListElementPtr Package::newListElement() {
        ListElementPtr r;

        if (listAvail != nullptr) {   // get node from avail chain if possible
            r = listAvail;
	        listAvail = listAvail->next;
        } else {            // otherwise allocate 2000 new nodes
            r = new ListElement[CHUNK_SIZE];
            allocated_list_chunks.push_back(r);
            ListElementPtr r2 = r + 1;
	        listAvail = r2;
            for (int i = 0; i < CHUNK_SIZE-2; i++, r2++) {
                r2->next = r2+1;
            }
            r2->next = nullptr;
        }
        return r;
    }


    void Package::toDot(Edge e, std::ostream& oss, bool isVector) {
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
        first = newListElement();
        first->p = e.p;
        first->next = nullptr;
        first->w = 0;

        unsigned short n=0, i=0;

        nodes << "\"R\"";
        //füge Kante zwischen helper node und neuem Knoten hinzu
        if (CN::equalsOne(e.w)) {
            nodes << " [label=\"\", shape=point];\n";
            edges << "\"R\" -> \"0\"\n";
        } else {
            nodes << " [label=\"\", shape=point];\n";
            edges << R"("R" -> "0" [label="()" << e.w << ")\" ];\n";
        }


        pnext = first;
        while (pnext != nullptr) {
            /* Zeichne Knoten*/
            nodes << "\"" << i << "\" " << "[ label=\""
            << pnext->p->v
            << "\" ,style=filled, fillcolor=lightgray ];\n";

            if (pnext->p != DDzero.p) {
                edges << "{rank=same;";
                for (unsigned int k = 0; k < NEDGE; k++) {
                	if (isVector && k % RADIX != 0) continue;
                    edges << " \"" << i << "h" << k << "\"";
                }
                edges << "}\n";

                for (int j = 0; j < NEDGE; j++) {
                	if (isVector && j % RADIX != 0) continue;
                    if (pnext->p->e[j].p == nullptr);
                    else {
                        if (!isTerminal(pnext->p->e[j])) {
                            q = first->next;
                            lastq = first;
                            while (q != nullptr && pnext->p->e[j].p != q->p) {
                                lastq = q;
                                q = q->next;
                            }
                            if (q == nullptr) {
                                q = newListElement();
                                q->p = pnext->p->e[j].p;
                                q->next = nullptr;
                                q->w = n = n + 1;
                                lastq->next = q;
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
                            if (CN::equalsOne(pnext->p->e[j].w)) {
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
                            if (CN::equalsZero(pnext->p->e[j].w)) {
                                nodes << ", fillcolor=red, color=red";
                            } else if (CN::equalsOne(pnext->p->e[j].w)) {
                                edges << "\"" << i << "h" << j << "\"-> \"T\";\n";
                            } else {
                                edges << "\"" << i << "h" << j << R"("-> "T" [label= "()" << pnext->p->e[j].w << ")\", ];\n";
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
    void Package::export2Dot(Edge basic, const std::string& outputFilename, bool isVector, bool show) {
        std::ofstream init("dot/"+outputFilename);
	    toDot(basic, init, isVector);
        init.close();
        if (show) {
            std::ostringstream oss;
            oss << "dot -Tsvg " << "dot/" + outputFilename << " -o " << "dot/" + outputFilename.substr(0, outputFilename.find_last_of('.')) << ".svg";
            auto str = oss.str(); // required to avoid immediate deallocation of temporary
            static_cast<void>(!std::system(str.c_str())); // cast and ! just to suppress the unused result warning
        }
    }


    Edge Package::normalize(Edge& e, bool cached) {
        int argmax = -1;
        bool zero[] = { CN::equalsZero(e.p->e[0].w),
                        CN::equalsZero(e.p->e[1].w)};

        for (int i = 0; i < NEDGE; i++) {
            if (zero[i] && e.p->e[i].w != CN::ZERO) {
                cn.releaseCached(e.p->e[i].w);
                e.p->e[i] = DDzero;
            }
        }

        //bool all_same = 1;
        //for (int i = 1; i < NEDGE; i++) {
        //    if (!equals(e.p->e[i], e.p->e[0])) {
        //        all_same = 0;
        //        break;
        //    }
        //}

        /*
        if (all_same) {
            //std::cout << "two succ are same" << std::endl;
            if (cached) {
                for (auto const& i : e.p->e) {
                    if (i.w != CN::ZERO && i.w != CN::ONE) {
                        cn.releaseCached(i.w);
                    }
                }
            }
            e.p->e[0].w = cn.lookup(e.p->e[0].w);
            return e.p->e[0];
        }
        */

        fp max = 0.L;
        Complex maxc = ComplexNumbers::ONE;
        // determine max amplitude
        for (int i = 0; i < NEDGE; ++i) {
            if (zero[i]) continue;
            if (argmax == -1) {
                argmax = i;
                max = ComplexNumbers::mag2(e.p->e[i].w);
                maxc = e.p->e[i].w;
            }
            else {
                auto mag = ComplexNumbers::mag2(e.p->e[i].w);
                if (mag - max > CN::TOLERANCE) {
                    argmax = i;
                    max = mag;
                    maxc = e.p->e[i].w;
                }
            }
        }

        // all equal to zero - make sure to release cached numbers approximately zero, but not exactly zero
        if (argmax == -1) {
            if (cached) {
                for (auto const& i : e.p->e) {
                    if (i.w != CN::ZERO) {
                        cn.releaseCached(i.w);
                    }
                }
            }
            return DDzero;
        }

        // divide each entry by max
        for (int i = 0; i < NEDGE; ++i) {
            if (i == argmax) {
                if (cached) {
                    if (e.w == ComplexNumbers::ONE)
                        e.w = maxc;
                    else
                        CN::mul(e.w, e.w, maxc);
                }
                else {
                    if (e.w == ComplexNumbers::ONE) {
                        e.w = maxc;
                    }
                    else {
                        auto c = cn.getTempCachedComplex();
                        CN::mul(c, e.w, maxc);
                        e.w = cn.lookup(c);
                    }
                }
                e.p->e[i].w = ComplexNumbers::ONE;

            }
            else {
                if (zero[i]) {
                    if (cached && e.p->e[i].w != ComplexNumbers::ZERO)
                        cn.releaseCached(e.p->e[i].w);
                    e.p->e[i] = DDzero;
                    continue;
                }
                Edge old_e = e.p->e[i];

                if (cached && !zero[i] && e.p->e[i].w != ComplexNumbers::ONE) {
                    cn.releaseCached(e.p->e[i].w);
                }
                if (CN::equalsOne(e.p->e[i].w)) {
                    e.p->e[i].w = ComplexNumbers::ONE;
                }
                auto c = cn.getTempCachedComplex();

                CN::div(c, e.p->e[i].w, maxc);

                e.p->e[i].w = cn.lookup(c);
            }

        }
        return e;
    }

	//  lookup a node in the unique table for the appropriate variable - if not found insert it
	//  only normalized nodes shall be stored.
	Edge& Package::UTlookup(Edge& e) {
		// there is a unique terminal node
		if (isTerminal(e)) {
            return e;
        }
        UTlookups++;

        std::uintptr_t key = 0;
        // note hash function shifts pointer values so that order is important
        // suggested by Dr. Nigel Horspool and helps significantly
        for (unsigned int i = 0; i < NEDGE; i++) {
            key += ((std::uintptr_t) (e.p->e[i].p) >> i)
                   + ((std::uintptr_t) (e.p->e[i].w.r) >> i)
                   + ((std::uintptr_t) (e.p->e[i].w.i) >> (i + 1));
        }
        key = key & HASHMASK;

        unsigned short v = varOrder[e.p->v];
        //std::cout << v << "  " << key << std::endl;
        NodePtr p = Unique[v][key]; // find pointer to appropriate collision chain
        while (p != nullptr)    // search for a match
        {
            if (std::memcmp(e.p->e, p->e, NEDGE * sizeof(Edge)) == 0) {
                // Match found
                //std::cout << "Match found" << std::endl;
                e.p->next = nodeAvail;    // put node pointed to by e.p on avail chain
                nodeAvail = e.p;

                // NOTE: reference counting is to be adjusted by function invoking the table lookup
                UTmatch++;        // record hash table match

                e.p = p;// and set it to point to node found (with weight unchanged)
                return e;
            }

            UTcol++;        // record hash collision
            p = p->next;
        }
        e.p->next = Unique[v][key]; // if end of chain is reached, this is a new node
        Unique[v][key] = e.p;       // add it to front of collision chain

        nodecount++;          // count that it exists
        if (nodecount > peaknodecount)
	        peaknodecount = nodecount;

        //checkSpecialMatrices(e.p);

        return e;                // and return
    }

    // set compute table to empty and
    // set toffoli gate table to empty and
    // set identity table to empty
    void Package::initComputeTable() {
        for (unsigned int i = 0; i < CTSLOTS; i++) {
            for (auto & table : CTable1) {
                table[i].r.p = nullptr;
                table[i].which = none;
            }
            for (auto & table : CTable2) {
                table[i].r = nullptr;
                table[i].which = none;
            }
            for (auto & table : CTable3) {
                table[i].r = nullptr;
                table[i].which = none;
            }
            for (auto& table : CTable4) {
                table[i].r= nullptr;
                table[i].var_cont = {};
            }
        }
        for (auto & i : TTable) {
            i.e.p = nullptr;
        }
        for (auto & i : IdTable) {
            i.p = nullptr;
        }
    }

	// a simple garbage collector that removes nodes with 0 ref count from the unique
	// tables placing them on the available space chain
	void Package::garbageCollect(bool force)
    {
        gc_calls++;
	    if (!force && nodecount < currentNodeGCLimit && cn.count < currentComplexGCLimit) {
		    return; // do not collect if below current limits
	    }
        gc_runs++;

	    int count = 0;
        int counta = 0;
        for (auto & variable : Unique) {
            for (auto & bucket : variable) {
                NodePtr lastp = nullptr;
                NodePtr p = bucket;
                while (p != nullptr) {
                    if (p->ref == 0) {
                        if (p == terminalNode){
                            std::cerr << "[ERROR] Tried to collect a terminal node\n";
                        }
                        count++;
                        NodePtr nextp = p->next;
                        if (lastp == nullptr)
                            bucket = p->next;
                        else
                            lastp->next = p->next;
                        p->next = nodeAvail;
	                    nodeAvail = p;
                        p = nextp;
                    } else {
                        lastp = p;
                        p = p->next;
                        counta++;
                    }
                }
            }
        }
	    currentNodeGCLimit += GCLIMIT_INC;
	    nodecount = counta;
	    cn.garbageCollect(); // NOTE: this cleans all complex values with ref-count 0
	    currentComplexGCLimit += ComplexNumbers::GCLIMIT_INC;
	    initComputeTable(); // IMPORTANT sets compute table to empty after garbage collection
    }

    // get memory space for a node
    NodePtr Package::getNode() {
        NodePtr r;

        if (nodeAvail != nullptr)    // get node from avail chain if possible
        {
            r = nodeAvail;
	        nodeAvail = nodeAvail->next;
        } else {            // otherwise allocate 2000 new nodes
	        r = new Node[CHUNK_SIZE];
            allocated_node_chunks.push_back(r);
            NodePtr r2 = r + 1;
	        nodeAvail = r2;
	        for (int i = 0; i < CHUNK_SIZE-2; i++, r2++) {
                r2->next = r2+1;
            }
            r2->next = nullptr;
        }
        r->next = nullptr;
        r->ref = 0;            // set reference count to 0
	    r->ident = r->symm = false; // mark as not identity or symmetric
        return r;
    }

    // increment reference counter for node e points to
    // and recursively increment reference counter for
    // each child if this is the first reference
    //
    // a ref count saturates and remains unchanged if it has reached
    // MAXREFCNT
    void Package::incRef(Edge& e) {
	    dd::ComplexNumbers::incRef(e.w);
        if (isTerminal(e))
            return;

        if (e.p->ref == MAXREFCNT) {
            std::cerr << "[WARN] MAXREFCNT reached for e.w=" << e.w << ". Weight will never be collected.\n";
	        //debugnode(e.p);
            return;
        }
        e.p->ref++;

        if (e.p->ref == 1) {
            for (auto& edge : e.p->e)
	            if (edge.p != nullptr) {
		            incRef(edge);
                }

            active[varOrder[e.p->v]]++;
            activeNodeCount++;
            maxActive = std::max(maxActive, activeNodeCount);
        }
    }

    // decrement reference counter for node e points to
    // and recursively decrement reference counter for
    // each child if this is the last reference
    //
    // a ref count saturates and remains unchanged if it has reached
    // MAXREFCNT
    void Package::decRef(Edge& e) {
	    dd::ComplexNumbers::decRef(e.w);

        if (isTerminal(e)) return;
        if (e.p->ref == MAXREFCNT) return;

        if (e.p->ref == 0) // ERROR CHECK
        {
            std::cerr <<"[ERROR] In decref: " << e.p->ref << " before decref\n";
	        //debugnode(e.p);
            std::exit(1);
        }
        e.p->ref--;

        if (e.p->ref == 0) {
	        for (auto& edge : e.p->e) {
	            if (edge.p != nullptr) {
		            decRef(edge);
	            }
	        }

            active[varOrder[e.p->v]]--;
            if (active[varOrder[e.p->v]] < 0) {
                std::cerr << "ERROR in decref\n";
                std::exit(1);
            }
            activeNodeCount--;
        }
    }

    unsigned int Package::nodeCount(const Edge& e, std::unordered_set<NodePtr>& v) const {
        v.insert(e.p);

        unsigned int sum = 1;
        if (!isTerminal(e)) {
            for (const auto & edge : e.p->e) {
                if (edge.p != nullptr && !v.count(edge.p)) {
                    sum += nodeCount(edge, v);
                }
            }
        }
        return sum;
    }

    unsigned int Package::size(const Edge& e) {
        visited.clear();
        return nodeCount(e, visited);
    }

	Edge Package::CTlookup(const Edge& a, const Edge& b, const CTkind which) {
    // Lookup a computation in the compute table
    // return NULL if not a match else returns result of prior computation
        Edge r{nullptr, {nullptr, nullptr}};
        CTlook[which]++;

        if (which == mult || which == fid || which == kron) {
            std::array<CTentry2, CTSLOTS>& table = CTable2.at(mode);
            const unsigned long i = CThash(a, b, which);

            if (table[i].which != which) return r;
            if (!equals(table[i].a, a)) return r;
	        if (!equals(table[i].b, b)) return r;

            CThit[which]++;
            r.p = table[i].r;

            if (std::fabs(table[i].rw.r) < CN::TOLERANCE && std::fabs(table[i].rw.i) < CN::TOLERANCE) {
                return DDzero;
            } else {
                r.w = cn.getCachedComplex(table[i].rw.r, table[i].rw.i);
            }

            return r;
        } else if (which == ad || which == noise || which == noNoise) {
            std::array<CTentry3, CTSLOTS>& table = CTable3.at(mode);
            ComplexValue aw{ a.w.r->val, a.w.i->val};
            ComplexValue bw{ b.w.r->val, b.w.i->val };
            const unsigned long i = CThash2(a.p, aw, b.p, bw, which);

            if (table[i].which != which) return r;
            if (table[i].a != a.p || !CN::equals(table[i].aw, aw)) return r;
            if (table[i].b != b.p || !CN::equals(table[i].bw, bw)) return r;

            CThit[which]++;
            r.p = table[i].r;

            if (std::fabs(table[i].rw.r) < CN::TOLERANCE && std::fabs(table[i].rw.i) < CN::TOLERANCE) {
                return DDzero;
            } else {
	            r.w = cn.getCachedComplex(table[i].rw.r, table[i].rw.i);
            }

            return r;
        } else if (which == conjTransp || which == transp) {
            std::array<CTentry1, CTSLOTS>& table = CTable1.at(mode);
            const unsigned long i = CThash(a, b, which);

            if (table[i].which != which) return r;
	        if (!equals(table[i].a, a)) return r;
	        if (!equals(table[i].b, b)) return r;

            CThit[which]++;
            return table[i].r;
        } else {
            std::cerr << "Undefined kind in CTlookup: " << which << "\n";
            std::exit(1);
        }
    }

    // put an entry into the compute table
    void Package::CTinsert(const Edge& a, const Edge& b, const Edge& r, const CTkind which) {
        if (which == mult || which == fid || which == kron) {
            if (CN::equalsZero(a.w) || CN::equalsZero(b.w)) {
                std::cerr << "[WARN] CTinsert: Edge with near zero weight a.w=" << a.w << "  b.w=" << b.w << "\n";
            }
            assert(((std::uintptr_t)r.w.r & 1u) == 0 && ((std::uintptr_t)r.w.i & 1u) == 0);
            std::array<CTentry2, CTSLOTS>& table = CTable2.at(mode);
            const unsigned long i = CThash(a, b, which);

            table[i].a = a;
            table[i].b = b;
            table[i].which = which;
            table[i].r = r.p;
            table[i].rw.r = r.w.r->val;
            table[i].rw.i = r.w.i->val;
        } else if (which == ad || which == noise || which == noNoise) {
            std::array<CTentry3, CTSLOTS>& table = CTable3.at(mode);
	        ComplexValue aw{ a.w.r->val, a.w.i->val };
	        ComplexValue bw{ b.w.r->val, b.w.i->val };
            const unsigned long i = CThash2(a.p, aw, b.p, bw, which);

            //assert(((std::uintptr_t)r.w.r & 1u) == 0 && ((std::uintptr_t)r.w.i & 1u) == 0);

            table[i].a = a.p;
            table[i].aw = aw;
            table[i].b = b.p;
            table[i].bw = bw;
            table[i].r = r.p;
            table[i].rw.r = r.w.r->val;
            table[i].rw.i = r.w.i->val;
            table[i].which = which;

        } else if (which == conjTransp || which == transp) {
            std::array<CTentry1, CTSLOTS>& table = CTable1.at(mode);
            const unsigned long i = CThash(a, b, which);

            table[i].a = a;
            table[i].b = b;
            table[i].which = which;
            table[i].r = r;
        } else {
            std::cerr << "Undefined kind in CTinsert: " << which << "\n";
            std::exit(1);
        }
    }

    Edge Package::CTlookup4(const Edge& a, const Edge& b, const std::vector<std::string>* var_cont)
    {
        Edge r{ nullptr, {nullptr, nullptr} };
        CTlook[con]++;
        std::array<CTentry4, CTSLOTS>& table = CTable4.at(mode);

        const unsigned long i = CThash4(a, b, var_cont);

        if (!equals(table[i].a, a)) return r;
        if (!equals(table[i].b, b)) return r;
  /*      for (int k = 0; k < table[i].var_cont.size(); k++) {
            std::cout << "aaaa" << std::endl;
            std::cout << table[i].var_cont.at(k) << std::endl;
            std::cout << "bbbb" << std::endl;
        }
        for (int k = 0; k < var_cont->size(); k++) {
            std::cout << "cccc" << std::endl;
            std::cout << var_cont->at(k) << std::endl;
            std::cout << "dddd" << std::endl;
        }*/

        std::vector<std::string> temp_var_cont;
        temp_var_cont.assign(var_cont->begin(), var_cont->end());
        if (table[i].var_cont != temp_var_cont) return r;

        CThit[con]++;
        r.p = table[i].r;

        if (std::fabs(table[i].rw.r) < CN::TOLERANCE && std::fabs(table[i].rw.i) < CN::TOLERANCE) {
            return DDzero;
        }
        else {
            r.w = cn.getCachedComplex(table[i].rw.r, table[i].rw.i);
        }

        return r;
    }
 
    void Package::CTinsert4(const Edge& a, const Edge& b, const Edge& r, const std::vector<std::string>* var_cont)
    {
        if (CN::equalsZero(a.w) || CN::equalsZero(b.w)) {
            std::cerr << "[WARN] CTinsert: Edge with near zero weight a.w=" << a.w << "  b.w=" << b.w << "\n";
            return;
        }
        
        assert(((std::uintptr_t)r.w.r & 1u) == 0 && ((std::uintptr_t)r.w.i & 1u) == 0);

        std::array<CTentry4, CTSLOTS>& table = CTable4.at(mode);

        const unsigned long i = CThash4(a, b, var_cont);

        table[i].a = a;
        table[i].b = b;
        table[i].var_cont.assign(var_cont->begin(), var_cont->end());
        table[i].r = r.p;
        table[i].rw.r = r.w.r->val;
        table[i].rw.i = r.w.i->val;
    }

    void Package::statistics() {
        auto hitRatioAdd = CTlook[ad] == 0 ? 0 : (double)CThit[ad] / (double)CTlook[ad];
        auto hitRatioMul = CTlook[con] == 0 ? 0 : (double)CThit[con] / (double)CTlook[con];
        auto hitRatioKron = ((CTlook[kron] == 0) ? 0 : (double)CThit[kron] / (double)CTlook[kron]);


        std::cout << "\nDD statistics:"
            << "\n  Current # nodes in UniqueTable: " << nodecount
            << "\n  Total compute table lookups: " << CTlook[0] + CTlook[1] + CTlook[2]+CTlook[3]
            << "\n  Number of operations:"
            << "\n    add:  " << nOps[T_ad]
            << "\n    mult: " << nOps[con]
            << "\n    kron: " << nOps[kron]
            << "\n  Compute table hit ratios (hits/looks/ratio):"
            << "\n    adds: " << CThit[ad] << " / " << CTlook[ad] << " / " << hitRatioAdd
            << "\n    mult: " << CThit[con] << " / " << CTlook[con] << " / " << hitRatioMul
            << "\n    kron: " << CThit[kron] << " / " << CTlook[kron] << " / " << hitRatioKron
            << "\n  UniqueTable:"
            << "\n    Collisions: " << UTcol
            << "\n    Matches:    " << UTmatch
            << "\n" << std::flush;
    }



    // make a DD nonterminal node and return an edge pointing to it
    // node is not recreated if it already exists
    Edge Package::makeNonterminal(std::string v, const Edge *edge, const bool cached) {
        //std::cout << "nodeAvail " << nodeAvail << std::endl;
    	Edge e{ getNode(), CN::ONE};
        //std::cout << "top before " << e.p << std::endl;
        e.p->v = v;

        std::memcpy(e.p->e, edge, NEDGE * sizeof(Edge));
        e = normalize(e, cached); // normalize it
        //std::cout << "top before " << e.p << std::endl;
        e = UTlookup(e);  // look it up in the unique tables
        /*
        if (e.p == e.p->e[0].p || e.p == e.p->e[1].p)
        {
            std::cout << std::endl;
            std::cout << "top index " << e.p->v << std::endl;
            std::cout << "top after " << e.p << std::endl;
            std::cout << e.p->e[0].p << std::endl;
            std::cout << e.p->e[1].p << std::endl << std::endl;
            std::cout << std::endl;
        }
        */
        return e;          // return result
    }


    Package::Package(std::map<std::string, int> var) : cn(ComplexNumbers()) {
        mode = Mode::Vector;
	    initComputeTable();  // init computed table to empty
        currentNodeGCLimit = GCLIMIT1; // set initial garbage collection limit
	    currentComplexGCLimit = ComplexNumbers::GCLIMIT1;
	    visited.max_load_factor(10);

        varOrder = var;

    }

    Package::~Package() {
        for(auto chunk : allocated_list_chunks) {
            delete[] chunk;
        }
        for(auto chunk : allocated_node_chunks) {
            delete[] chunk;
        }
    }

    Edge Package::Slice(Edge x, std::string a, unsigned int c) {

        if (isTerminal(x) || varOrder[a] < varOrder[x.p->v]) {
            return x;
        }
        Edge res;
        if (varOrder[a] == varOrder[x.p->v]) {
            res = x.p->e[c];
            return res;
        }
    }

    Edge Package::T_add2(Edge x, Edge y){
       //几种终止情形
        if (x.p == nullptr) {
            return y;  
        }
        if (y.p == nullptr) {
            return x; 
        }
        nOps[T_ad]++;

        if (x.w == CN::ZERO) {
            if (y.w == CN::ZERO) {
                return y;
            }
            y.w = cn.getCachedComplex(y.w.r->val, y.w.i->val);
            return y;
        }
        if (y.w == CN::ZERO) {
            x.w = cn.getCachedComplex(x.w.r->val, x.w.i->val);
            return x;
        }
        if (x.p == y.p) {
            Edge r = y;
            r.w = cn.addCached(x.w, y.w);
            if (CN::equalsZero(r.w)) {
                cn.releaseCached(r.w);
                return DDzero;
            }
            return r;
        }
        Edge r = CTlookup(x, y, ad);
        if (r.p != nullptr) {
            return r;
        }

        std::string w;
        if (isTerminal(x)) {
            w = y.p->v;
        }
        else {
            w = x.p->v;
            if (!isTerminal(y) && varOrder[y.p->v] < varOrder[w]) {
                w = y.p->v;
            }
        }

        Edge e1{}, e2{}, e[NEDGE];
        for (int i = 0; i < NEDGE; i++) {
            if (!isTerminal(x) && x.p->v == w) {
                e1 = x.p->e[i];

                if (e1.w != CN::ZERO) {
                    e1.w = cn.mulCached(e1.w, x.w);
                }
            }
            else {
                e1 = x;
                if (y.p->e[i].p == nullptr) {
                    e1 = { nullptr, CN::ZERO };
                }
            }
            if (!isTerminal(y) && y.p->v == w) {
                e2 = y.p->e[i];

                if (e2.w != CN::ZERO) {
                    e2.w = cn.mulCached(e2.w, y.w);
                }
            }
            else {
                e2 = y;
                if (x.p->e[i].p == nullptr) {
                    e2 = { nullptr, CN::ZERO };
                }
            }

            e[i] = T_add2(e1, e2);

            if (!isTerminal(x) && x.p->v == w && e1.w != CN::ZERO) {
                cn.releaseCached(e1.w);
            }

            if (!isTerminal(y) && y.p->v == w && e2.w != CN::ZERO) {
                cn.releaseCached(e2.w);
            }
        }

        r = makeNonterminal(w, e, true);

        CTinsert(x, y, r, ad);

        return r;
    }

    Edge Package::T_add(Edge x, Edge y) {
        const auto before = cn.cacheCount;
        Edge result = T_add2(x, y);

        if (result.w != CN::ZERO) {
            cn.releaseCached(result.w);
            result.w = cn.lookup(result.w);
        }
        const auto after = cn.cacheCount;
        assert(after == before);
        return result;
    }

    TDD Package::cont(TDD tdd1, TDD tdd2, int ttt) {

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

        std::sort(var_cont.begin(), var_cont.end());

        const auto before = cn.cacheCount;

        TDD res;
        res.index_set = var_out;

        var_out.clear();
        var_cont_temp.clear();
        
        var_out_key.clear();

        //std::cout << "---" << std::endl;
        //for (int k = 0; k < var_cont.size(); k++) {

        //    std::cout << var_cont[k] << std::endl;
        //}
        //std::cout << "---" << std::endl;

        //if (ttt == 1) {
        //    dd::Edge t1 = Slice(tdd1.e, "x301", 0);
        //    t1 = Slice(t1, "x300", 0);
        //    t1 = Slice(t1, "x202", 0);
        //    t1 = Slice(t1, "x200", 0);
        //    dd::Edge t2 = Slice(tdd2.e, "x301", 0);
        //    t2 = Slice(t2, "x102", 0);
        //    export2Dot(t1, "t1", false);
        //    export2Dot(t2, "t2", false);
        //    res.e = cont2(t1, t2, var_cont, var_cont.size(), ttt);
        //}
        //else {
            res.e = cont2(tdd1.e, tdd2.e, &var_cont, var_cont.size(), ttt);
        //}
        
        //export2Dot(res.e, "res", false);
        var_cont.clear();

        if (res.e.w != ComplexNumbers::ZERO && res.e.w != ComplexNumbers::ONE) {
            cn.releaseCached(res.e.w);
            res.e.w = cn.lookup(res.e.w);
        }
        const auto after = cn.cacheCount;
        assert(before == after);

        return res;
    }


   Edge Package::cont2(Edge x, Edge y, const std::vector<std::string>* var_cont, int var_num,int ttt) {

       if (x.p == nullptr)
           return x;
       if (y.p == nullptr)
           return y;
        nOps[con]++;

        //if (ttt == 1) {
        //    std::cout << "---------" << std::endl;
        //    std::cout << x.p->v << std::endl;
        //    std::cout << y.p->v << std::endl;
        //    for (int k = 0; k < var_cont.size(); k++) {
        //        std::cout << var_cont.at(k) << std::endl;
        //    }
        //}

        if (x.w == CN::ZERO || y.w == CN::ZERO|| equals(x,DDzero)||equals(y,DDzero)) {
            //if (ttt == 1) {
            //    std::cout << "case1" << std::endl;
            //}
            return DDzero;
        }

        if (isTerminal(x) and isTerminal(y))
        {
            //if (ttt == 1) {
            //    std::cout << "case2" << std::endl;
            //}
            Edge res;
            auto c = cn.getCachedComplex(pow(2, var_num), 0);
            CN::mul(c, c, x.w);
            CN::mul(c, c, y.w);
            res=makeTerminal(c);
            if (CN::equalsZero(res.w)) {
                cn.releaseCached(res.w);
                res = DDzero;
            }

            return res;
        }

        if (isTerminal(x)&& var_num==0) {
            //if (ttt == 1) {
            //    std::cout << "case3" << std::endl;
            //}
            Edge res;
            res= y;
            res.w= cn.mulCached(x.w, y.w);
            if (CN::equalsZero(res.w)) {
                cn.releaseCached(res.w);
                res = DDzero;
            }

            return res;
        }

        if (isTerminal(y)) {
            //if (ttt == 1) {
            //    std::cout << "case4" << std::endl;
            //}
            return cont2(y, x,var_cont, var_num);
        }

        const Complex xweight = x.w;
        const Complex yweight = y.w;
        x.w = CN::ONE;
        y.w = CN::ONE;

        Edge r = CTlookup4(x, y, var_cont);
        if (r.p != nullptr) {
            //if (ttt == 1) {
            //    export2Dot(x, "x", false);
            //    export2Dot(y, "y", false);
            //    export2Dot(r, "r", false);
                //std::cout << "case5" << std::endl;
            //}
            if (r.w != CN::ZERO) {
                CN::mul(r.w, r.w, xweight);
                CN::mul(r.w, r.w, yweight);
                if (CN::equalsZero(r.w)) {
                    cn.releaseCached(r.w);
                    return DDzero;
                }
            }
            return r;
        }

        std::string v_top;
        if (varOrder[x.p->v] < varOrder[y.p->v]) {
            v_top = x.p->v;
        }
        else {
            v_top = y.p->v;
        }


        Edge low{}, high{};
        if (find(var_cont->begin(), var_cont->end(), v_top) == var_cont->end()) {
            //if (ttt == 1) {
            //    std::cout << "case6" << std::endl;
            //}
            Edge low = cont2(Slice(x, v_top, 0), Slice(y, v_top, 0), var_cont, var_num ,ttt);
            Edge high = cont2(Slice(x, v_top, 1), Slice(y, v_top, 1), var_cont, var_num,ttt );
            //if (equals(low,high)) {
            //    if (low.w == CN::ZERO) {
            //        r = DDzero;
            //    }
            //    else {
            //        r = low;
            //        cn.releaseCached(high.w);
            //    }

            //}
            //else {
                Edge e[NEDGE];
                e[0] = low;
                e[1] = high;
                r = makeNonterminal(v_top, e, true);
            //}

        }
        else {
            //if (ttt == 1) {
            //    std::cout << "case7" << std::endl;
            //}
            Edge low = cont2(Slice(x, v_top, 0), Slice(y, v_top, 0), var_cont, var_num - 1,ttt);
            Edge high = cont2(Slice(x, v_top, 1), Slice(y, v_top, 1), var_cont, var_num - 1,ttt);
            if (low.w == CN::ZERO) {
                r = high;
            }
            else if(high.w == CN::ZERO){
                r = low;
            }
            else {
                r = T_add2(low, high);
                cn.releaseCached(low.w);
                cn.releaseCached(high.w);
            }
        }

        CTinsert4(x, y, r, var_cont);

        if (r.w != CN::ZERO && (xweight != CN::ONE || yweight != CN::ONE)) {
            if (r.w == CN::ONE) {
                r.w = cn.mulCached(xweight, yweight);
            }
            else {
                CN::mul(r.w, r.w, xweight);
                CN::mul(r.w, r.w, yweight);
            }
            if (CN::equalsZero(r.w)) {
                cn.releaseCached(r.w);
                r = DDzero;
            }

        }
        return r;
    }

    TDD Package::Matrix2TDD(const Matrix2x2& mat, std::vector<Index> var_out)
    {

        TDD low,high,res;
        Edge e_low[NEDGE], e_high[NEDGE],e[NEDGE],e_temp[RADIX* RADIX];

        for (int i = 0; i < RADIX; i++) {
            for (int j = 0; j < RADIX; j++) {
                if (mat[i][j].r == 0.0 && mat[i][j].i == 0.0) {
                    e_temp[i * RADIX + j] = DDzero;
                }
                else {

                    if (mat[i][j].r == 1.0 && mat[i][j].i == 0.0) {
                        e_temp[i * RADIX + j] = DDone;
                    }
                    else {
                        e_temp[i * RADIX + j] = makeTerminal(cn.lookup(mat[i][j]));
                    }
                }
            }
        }
        std::string v_top;
        std::string v_bottom;
        if (varOrder[var_out[0].key] < varOrder[var_out[1].key]) {
            v_top = var_out[0].key;
            v_bottom= var_out[1].key;
            e_low[0] = e_temp[0];
            e_low[1] = e_temp[2];
            e_high[0] = e_temp[1];
            e_high[1] = e_temp[3];
        }
        else {
             v_top= var_out[1].key;
             v_bottom = var_out[0].key;
            e_low[0] = e_temp[0];
            e_low[1] = e_temp[1];
            e_high[0] = e_temp[2];
            e_high[1] = e_temp[3];
        }


        if (equals(e_low[0],e_low[1])){
            low.e = e_low[0];
        }
        else {
            low.e = makeNonterminal(v_bottom, e_low);
        }
        if (equals(e_high[0], e_high[1])) {
            high.e = e_high[0];
        }
        else {
            high.e = makeNonterminal(v_bottom, e_high);
        }
        if (equals(low.e, high.e)) {
            res.e = low.e;
        }
        else {
            e[0] = low.e;
            e[1] = high.e;
            res.e = makeNonterminal(v_top, e);
        }
        res.index_set = var_out;
 
        return res;
    }

    TDD Package::diag_matrix_2_TDD(const Matrix2x2& mat, std::vector<Index> var_out)
    {

        TDD res;
        Edge e_temp[RADIX];

        for (int i = 0; i < RADIX; i++) {
            
                if (mat[i][i].r == 0.0 && mat[i][i].i == 0.0) {
                    e_temp[i] = DDzero;
                }
                else {
                    if (mat[i][i].r == 1.0 && mat[i][i].i == 0.0) { 
                        e_temp[i] = DDone;
                    }
                    else{ 
                        e_temp[i] = makeTerminal(cn.lookup(mat[i][i])); 
                    }
                    
                }
          
        }

        res.e = makeNonterminal(var_out[0].key, e_temp);

        res.index_set = var_out;

        return res;
    }

    TDD Package::cnot_2_TDD(std::vector<Index> var, int ca=1) {

        constexpr dd::Matrix2x2 Xmat = { {{ 0, 0 }, { 1, 0 } }, {{ 1, 0 }, { 0, 0 } } };
        constexpr dd::Matrix2x2 Imat = { {{ 1, 0 }, { 0, 0 } }, {{ 0, 0 }, { 1, 0 } } };
        std::string v_top;
        TDD low, high,res;
        Edge e[NEDGE];
        if (ca == 1) {
            if (varOrder[var[0].key] < varOrder[var[3].key] && varOrder[var[0].key] < varOrder[var[4].key]) {
                v_top = var[0].key;
                low = Matrix2TDD(Imat, { var[3] ,var[4] });
                high = Matrix2TDD(Xmat, { var[3] ,var[4] });
                res.e = makeNonterminal(v_top, {low.e,high.e});
                res.index_set = { var[0],var[2],var[3],var[4] };
                
            }
            else if (varOrder[var[3].key] < varOrder[var[0].key] && varOrder[var[3].key] < varOrder[var[4].key]) {
                v_top = var[3].key;
                low = Matrix2TDD(Imat, { var[0] ,var[4] });
                high = Matrix2TDD(Xmat, { var[0] ,var[4] });
                res.e = makeNonterminal(v_top, { low.e,high.e });
                res.index_set = { var[0],var[2],var[3],var[4] };
                
            }
            else {
                v_top = var[4].key;
                low = Matrix2TDD(Imat, { var[0] ,var[3] });
                high = Matrix2TDD(Xmat, { var[0] ,var[3] });
                res.e = makeNonterminal(v_top, { low.e,high.e });
                res.index_set = { var[0],var[2],var[3],var[4] };
                
            }
        }
        if (ca == 2) {
            res.e = DDone;
            res.index_set = { var[0],var[1],var[2] };
        }
        if (ca == 3) {
            if (varOrder[var[1].key] < varOrder[var[3].key] && varOrder[var[1].key] < varOrder[var[4].key]) {
                v_top = var[1].key;
                low = Matrix2TDD(Imat, { var[3] ,var[4] });
                high = Matrix2TDD(Xmat, { var[3] ,var[4] });
                res.e = makeNonterminal(v_top, { low.e,high.e });
                res.index_set = { var[1],var[3],var[4] };
                
            }
            else if (varOrder[var[3].key] < varOrder[var[1].key] && varOrder[var[3].key] < varOrder[var[4].key]) {
                v_top = var[3].key;
                low = Matrix2TDD(Imat, { var[1] ,var[4] });
                high = Matrix2TDD(Xmat, { var[1] ,var[4] });
                res.e = makeNonterminal(v_top, { low.e,high.e });
                res.index_set = { var[1],var[3],var[4] };
                
            }
            else {
                v_top = var[4].key;
                low = Matrix2TDD(Imat, { var[1] ,var[3] });
                high = Matrix2TDD(Xmat, { var[1] ,var[3] });
                res.e = makeNonterminal(v_top, { low.e,high.e });
                res.index_set = { var[1],var[3],var[4] };
                
            }
        }
        return res;
    }

}
