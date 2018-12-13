/**
 * @file MCParticle.h
 * @brief Class which implements an MC particle that stores information about
 *        tracks from the simulation.
 * @author: Omar Moreno, SLAC National Accelerator Laboratory
 */

#ifndef _MC_PARTICLE_H_
#define _MC_PARTICLE_H_

//----------------//
//   C++ StdLib   //
//----------------//
#include <iostream>
#include <map>
#include <string>
#include <vector>

//----------//
//   ROOT   //
//----------//
#include "TObject.h"
#include "TRefArray.h"

class MCParticle : public TObject {

    public:

        /** Constructor */
        MCParticle(); 

        /** Destructor */
        virtual ~MCParticle();

        /**        
         * Clear the data in this object.
         */
        void Clear(Option_t *option = "");

        /**
         * Print out information of this object.
         */
        void Print(Option_t *option = "") const;

        /**        
         * Get the energy of the particle [GeV].
         *
         * @return The energy of the particle.
         */
        double getEnergy() const { return energy_; }

        /**
         * Get the PDG code of the particle.
         *
         * @return The PDG code of the particle.
         */
        int getPdgID() const { return pdg_id_; }

        /**        
         * Get the generator status of the particle.
         *
         * @return The generator status.
         */
        int getGenStatus() const { return gen_status_; }

        /**
         * Get the global time of the particle's creation [ns].
         *   
         * @return The global time of the particle's creation.
         */
        double getTime() const { return time_; }

        /**
         * Get the XYZ vertex of the particle's creation [mm].
         *    
         * @return The vertex of the particle.
         */
        std::vector<double> getVertex() const { return {x_, y_, z_}; }


        /**
         * Get the endpoint of the particle where it was destroyed
         * or left the detector [mm].
         *
         * @return The endpoint of the particle
         */
        std::vector<double> getEndPoint() const { return {end_x_, end_y_, end_z_}; }

        /**
         * Get the XYZ momentum of the particle [MeV].
         *
         * @return The momentum of the particle.
         */
        std::vector<double> getMomentum() const { return {px_, py_, pz_}; }

        /**
         * Get the mass of the particle [GeV].
         *    
         * @return The mass of the particle.
         */
        double getMass() const { return mass_; }

        /**
         * The charge of the particle (units of electron charge).
         *    
         * @return The charge of the particle.
         */
        double getCharge() const { return charge_; }
        
        /**
         * Get the number of daughter particles.
         */
        int getDaughterCount() const {
            return daughters_->GetEntriesFast();
        }

        /**
         * Get a daughter particle by index.
         *
         * @param iDau The index of the daughter particle.
         */
        MCParticle* getDaughter(int daughter_index) {
            return static_cast<MCParticle*>(daughters_->At(daughter_index));
        }

        /**
         * Get the number of parent particles.
         *
         * @return The number of parent particles.
         */
        int getParentCount() const {
            return parents_->GetEntriesFast();
        }

        /**
         * Get a parent particle by index.
         *
         * @param iPar The index of the parent particle.
         */
        MCParticle* getParent(int parent_index) {
            return static_cast<MCParticle*>(parents_->At(parent_index));
        }

        /**
         * Set the energy of the particle [MeV].
         *
         * @param energy The energy of the particle.
         */
        void setEnergy(const double energy) { this->energy_ = energy; }

        /**
         * Set the PDG code of the hit.
         *    
         * @param pdg_id The PDG code of the hit.
         */
        void setPdgID(const int pdg_id) { this->pdg_id_ = pdg_id; }

        /**
         * Set the generator status of the hit.
         *    
         * @param get_status The generator status of the hit.
         */
        void setGenStatus(const int get_status) { this->gen_status_ = get_status; }

        /**
         * Set the global time of the particle's creation [ns].
         *    
         * @param time The global time of the particle's creation.
         */
        void setTime(const double time) { this->time_ = time; }

        /**
         * Set the vertex of the particle [mm].
         *
         * @param x The vertex X position.
         * @param y The vertex Y position.
         * @param z The vertex Z position.
         */
        void setVertex(const double x, const double y, const double z) {
            this->x_ = x;
            this->y_ = y;
            this->z_ = z;
        }

        /**
         * Set the end point of the particle [mm].
         *
         * @param end_x The X end point.
         * @param end_y The Y end point.
         * @param end_z The Z end point.
         */
        void setEndPoint(const double end_x, const double end_y, const double end_z) {
            this->end_x_ = end_x;
            this->end_y_ = end_y;
            this->end_z_ = end_z;
        }

        /**
         * Set the momentum of the particle [GeV].
         *
         * @param px The X momentum.
         * @param py The Y momentum.
         * @param pz The Z momentum.
         */
        void setMomentum(const double px, const double py, const double pz) {
            this->px_ = px;
            this->py_ = py;
            this->pz_ = pz;
        }

        /**
         * Set the mass of the particle [GeV].
         *
         * @param mass The mass of the particle.
         */
        void setMass(const double mass) { this->mass_ = mass; }

        /**
         * Set the charge of the particle.
         *
         * @param charge The charge of the particle.
         */
        void setCharge(const double charge) { this->charge_ = charge; }

        /**
         * Add a daughter particle.
         *
         * @param daughter The daughter particle.
         */
        void addDaughter(MCParticle* daughter) { daughters_->Add(daughter); }

        /**
         * Add a parent particle.
         *
         * @param parent The parent particle.
         */
        void addParent(MCParticle* parent) { parents_->Add(parent); }

    private:

        /** The energy of the particle. */
        double energy_{0};

        /** The PDG code of the particle. */
        int pdg_id_{0};

        /** The generator status. */
        int gen_status_{-1};

        /** The global creation time. */
        double time_{0};

        /** The X vertex. */
        double x_{0};

        /** The Y vertex. */
        double y_{0};

        /** The Z vertex. */
        double z_{0};

        /** The X end point. */
        double end_x_{0};

        /** The Y end point. */
        double end_y_{0};

        /** The Z end point. */
        double end_z_{0};

        /** The X momentum.*/
        double px_{0};

        /** The Y momentum. */
        double py_{0};

        /** The Z momentum. */
        double pz_{0};

        /** The particle's mass. */
        double mass_{0};

        /** The particle's charge. */
        double charge_{0};

        /** The list of daughter particles. */
        TRefArray* daughters_{new TRefArray};

        /** The list of parent particles. */
        TRefArray* parents_{new TRefArray};

        /**
         * ROOT class definition.
         */
        ClassDef(MCParticle, 1);

}; // MCParticle

#endif // _MC_PARTICLE_H_
