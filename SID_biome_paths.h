/* 
 * File:   SID_biome_paths.h
 * Author: geth2k
 *
 * Created on November 12, 2014, 6:06 PM
 */

#ifndef SID_BIOME_PATHS_H
#define	SID_BIOME_PATHS_H

#include <vector>
#include <iostream>

namespace sid {

    class biome_paths {
    public:

        class biome_path {
        public:

            class biome_bubble {
            public:

                /**@brief Constructor for the biome_bubble object.
                 * 
                 * @param x The X co-ordinate for the bubble within the region.
                 * @param z The Y co-ordinate for the bubble within the region.
                 * @param radius The radius of the bubble.
                 * @param biomeID The array index of the biome.
                 */
                biome_bubble(int x,
                        int z,
                        int radius,
                        int biomeID);

                /** @brief Copy constructor for the biome_bubble object.
                 */
                biome_bubble(biome_bubble const& orig);

                /** @brief Destructor for the biome_bubble object.
                 */
                virtual ~biome_bubble();
                
                bool is_hit(int x, int z) const;
                
                int get_biome_id() const;

                /** @brief The minimum radius, in plots, of a bubble. */
                static const int MIN_RADIUS = 90;
                
                /** @brief The maximum radius, in plots, of a bubble. */
                static const int MAX_RADIUS = 180;
                
                /** @brief Size of the range between min and max radius. */
                static const int RADIUS_RANGE = MAX_RADIUS - MIN_RADIUS;

            private:
                
                int __m_iRadius;
                int __m_iX;
                int __m_iZ;
                int __m_iBiomeID;
                
            };

            typedef std::vector<biome_bubble> t_bubbles;
            typedef t_bubbles::iterator t_bubbles_iter;
            typedef t_bubbles::const_iterator t_bubbles_const_iter;

            /** @brief Constructor for the biome_path object.
             * 
             * @param pos Array position of the Path.
             * @param regionX X coordinate of the parent Region.
             * @param regionZ Z coordinate of the parent Region.
             * @param regionWidth With of the parent Region.
             * @param minBiomeID Minimum Biome ID.
             * @param maxBiomeID Maximum Biome ID.
             * @param seed Seed for random generation.
             */
            biome_path(int pos,
                    int regionX,
                    int regionZ,
                    int regionWidth,
                    int minBiomeID,
                    int maxBiomeID,
                    int seed);

            /** @brief Copy constructor for the biome_path object.
             */
            biome_path(biome_path const& orig);

            /** @brief Move constructor for the biome_path object.
             */
            biome_path(biome_path&& orig);

            /** @brief Destructor for the biome_path object.
             */
            virtual ~biome_path();
            
            int get_biome_id(int x, int z) const;
            
            /** */
            static const int MIN_DEVIATION = 15;

            /** */
            static const int MAX_DEVIATION = 25;

            /** */
            static constexpr int DEVIATION_RANGE = MAX_DEVIATION - MIN_DEVIATION;            

        private:
            int __m_iPosition;
            t_bubbles __M_vBubbles;
        };

        typedef std::vector<biome_path> t_paths;
        typedef t_paths::iterator t_paths_iter;
        typedef t_paths::const_iterator t_paths_const_iter;

        /** @brief
         */
        biome_paths();

        /** @brief
         */
        biome_paths(biome_paths const& orig);

        /** @brief
         */
        virtual ~biome_paths();

        /** @brief
         * 
         * @param quant
         * @param x
         * @param z
         * @param regionWidth
         * @param minBiomeID
         * @param maxBiomeID
         * @param seed
         */
        void add_paths(int quant,
                int x,
                int z,
                int regionWidth,
                int minBiomeID,
                int maxBiomeID,
                int seed);
        
        /** @brief
         * 
         * @param x
         * @param z
         * @return 
         */
        int get_biome_id(int x, int z) const;

        /** */
        static const int MIN_PATHS = 9;
        
        /** */
        static const int MAX_PATHS = 18;
        
        /** */
        static constexpr int PATHS_RANGE = MAX_PATHS - MIN_PATHS;

    private:
        t_paths __m_vPaths;
    };
}

#endif	/* SID_BIOME_PATHS_H */