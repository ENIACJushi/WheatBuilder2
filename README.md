
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

4. biomeId：
- IDs here are a little different from the one described on the wiki, but the name is exactly the same, and due to some features, only ids from 0 to 50 can be used in commands.

#### > THANKS：
幻化成疯 Provides save chunk method.
