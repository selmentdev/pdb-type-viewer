# Sql Model

## TODO

[ ] Use SQLite as internal data format for project

## Required tables

[ ] TABLE `tv.types`
[ ] TABLE `tv.members`
[ ] TABLE `tv.data_sources` - this one is needed to "reimport" PDB files after they change
  - use cases:
    - user added new PDB file - need to import it, but this file may introduce possible conflicts (different compilation etc.)
    - user defines list of PDBs and reimport them all at once
  - app actions:
    - add / remove file from list
    - import data from single file / all files at once
