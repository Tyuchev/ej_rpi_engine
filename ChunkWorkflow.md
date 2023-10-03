# Chunk creation workflow
This is my process for creating chunks in Blender, written down here so I don't forget it.

1. Add a 1m plane, scale it by 9. This is the reference plane for how big the chunk is.
2. Import the models you want to use, hide the ones you don't need right now.
   * Delete the tmpParent for all of them.
3. Duplicate and move the models where you want them until you have what you want.
4. Use ruler to measure against x and z axes.
    * Use `ctrl` to snap to elements, make sure to pick consistent ones (it should visibly snap).
    * Use axis snap.
5. Use math to figure out how much to move everything in each axis so it's centered, and do so.
6. Measure again to check it's centered.
7. Join all models into one object.
8. Cursor to world origin, then both origin and pivot to cursor.
9. In *edit mode*, rotate chunk so the input direction faces +z. 
10. Select the chunk, and when exporting to gltf make sure to only export selected.
