
#### > Current supported version：1.20.0

#### > Commands

- Basic Edit Commands：
  - **be biome get [pos] [dimId]** 
    - Get biome at [pos],[dimId]
  - **be biome set_id [pos] [dimId] [biomeId]** 
    - Set biome at [pos],[dimId] by ID.
  - **be biome set_name [pos] [dimId] [biomeName]** 
    - Set biome at [pos],[dimId] by name.
  - **be biomes replace [startPos] [endPos] [dimId] [originId] [replaceId]** 
    - Replace biomes in rectangle area to [replaceId] whose id equals [replaceId], if originId equals -1, it would replace all.
  - **be biomes replace [startPos] [endPos] [dimId] [originName] [replaceName]** 
    - Replace biomes in rectangle area to [replaceName] whose name equals [replaceName], if [originName] equals "every", it would replace all.

- Save and load with chunk as the smallest unit：
  - **mvc save [start_x] [start_z] [end_x] [end_z] [dim_id] [file_name] [save]** 
    - Save the biomes of all chunks with chunk coordinates between [start_x] [start_z] and [end_x] [end_z] (including boundaries).
    - Directory is "plugins/WheatBuilder/Area/file_name".
    - Ungenerated chunks will automatically generated and saved to the world when [save] is true.
    - Note：[dim_id] can currently only be 0 (overworld).

  - **mvc load [start_x] [start_z] [dim_id] [file_name]**
    - Load the saved chunk biomes out, starting with [start_x] [start_z].
    - Ungenerated chunks will automatically generated and saved to the world.
    - Note：[dim_id] can currently only be 0 (overworld).
  - **mvc generate [start_x] [start_z] [end_x] [end_z] [dim_id]**
    - Generate chunks, and save them to the world.

#### > Useful message

1. Changes to the biome will require re-entry into the world to be observed.
2. dimId：0 OverWorld；1 Nether；2 The End；
3. Chunk and chunk coordinate
- chunk：https://minecraft.fandom.com/wiki/Chunk
- chunk coordinate： The chunk coordinate of a block at (x, z) is (x/16, z/16), rounded down.
- Example: block (3, 64, 3) is in the chunk (0, 0).

#### > biomeId：
- IDs here are a little different from the one described on the wiki, but the name is exactly the same, and due to some features, only ids from 0 to 50 can be used in commands.

| ID   | name                             |
| ---- | -------------------------------- |
| 0    | ocean                            |
| 1    | plains                           |
| 2    | desert                           |
| 3    | extreme_hills                    |
| 4    | forest                           |
| 5    | taiga                            |
| 6    | swampland                        |
| 7    | river                            |
| 8    | hell                             |
| 9    | the_end                          |
| 10   | legacy_frozen_ocean              |
| 11   | frozen_river                     |
| 12   | ice_plains                       |
| 13   | ice_mountains                    |
| 14   | mushroom_island                  |
| 15   | mushroom_island_shore            |
| 16   | beach                            |
| 17   | desert_hills                     |
| 18   | forest_hills                     |
| 19   | taiga_hills                      |
| 20   | extreme_hills_edge               |
| 21   | jungle                           |
| 22   | jungle_hills                     |
| 23   | jungle_edge                      |
| 24   | deep_ocean                       |
| 25   | stone_beach                      |
| 26   | cold_beach                       |
| 27   | birch_forest                     |
| 28   | birch_forest_hills               |
| 29   | roofed_forest                    |
| 30   | cold_taiga                       |
| 31   | cold_taiga_hills                 |
| 32   | mega_taiga                       |
| 33   | mega_taiga_hills                 |
| 34   | extreme_hills_plus_trees         |
| 35   | savanna                          |
| 36   | savanna_plateau                  |
| 37   | mesa                             |
| 38   | mesa_plateau_stone               |
| 39   | mesa_plateau                     |
| 40   | warm_ocean                       |
| 42   | lukewarm_ocean                   |
| 43   | deep_lukewarm_ocean              |
| 44   | cold_ocean                       |
| 45   | deep_cold_ocean                  |
| 46   | frozen_ocean                     |
| 47   | deep_frozen_ocean                |
| 48   | bamboo_jungle                    |
| 49   | bamboo_jungle_hills              |
| 129  | sunflower_plains                 |
| 130  | desert_mutated                   |
| 131  | extreme_hills_mutated            |
| 132  | flower_forest                    |
| 133  | taiga_mutated                    |
| 134  | swampland_mutated                |
| 140  | ice_plains_spikes                |
| 149  | jungle_mutated                   |
| 151  | jungle_edge_mutated              |
| 155  | birch_forest_mutated             |
| 156  | birch_forest_hills_mutated       |
| 157  | roofed_forest_mutated            |
| 158  | cold_taiga_mutated               |
| 160  | redwood_taiga_mutated            |
| 161  | redwood_taiga_hills_mutated      |
| 162  | extreme_hills_plus_trees_mutated |
| 163  | savanna_mutated                  |
| 164  | savanna_plateau_mutated          |
| 165  | mesa_bryce                       |
| 166  | mesa_plateau_stone_mutated       |
| 167  | mesa_plateau_mutated             |
| 178  | soulsand_valley                  |
| 179  | crimson_forest                   |
| 180  | warped_forest                    |
| 181  | basalt_deltas                    |
| 182  | jagged_peaks                     |
| 183  | frozen_peaks                     |
| 184  | snowy_slopes                     |
| 185  | grove                            |
| 186  | meadow                           |
| 187  | lush_caves                       |
| 188  | dripstone_caves                  |
| 189  | stony_peaks                      |
| 190  | deep_dark                        |
| 191  | mangrove_swamp                   |
| 192  | cherry_grove                     |


#### > THANKS：
幻化成疯 Provides save chunk method.
