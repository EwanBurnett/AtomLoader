# Atom Loader
A utility to convert 3D model files into a binary format, and a library to load the binary into a convenient struct.

Supports all formats used by [Assimp](https://github.com/assimp/assimp/blob/master/doc/Fileformats.md)

## Usage 

CLI
```
Atom-Loader.exe file1 file2 ... 
```

## Atom Binary Format
Atom serializes models into a fast-loading binary format (.Atom), and provides methods to parse this format into an `Atom::Model` struct. However, Implementing applications may choose to parse this format manually. As such, below is the schema of the format.

### Metadata
The header of the binary contains various metadata, used to validate the file. 


| Element | Description | Size (Bytes) |
| ------- | ----------- | ------------ |
| Version | The version of Atom used to serialize the file. | 4 |
| Source File | The filename of the model's source. | 8 + (string length) |
| Hash ID | The SHA-256 Hashed ID of the model. | 32 |
| Mesh Count | How many meshes are in this model. | 8 |
| Mesh Data | The data for each individual mesh - Written once per Mesh | variable: min 64 |


### Mesh Data
Uncompressed data corresponding to that of an `Atom::Mesh`, representing the raw vertex data of an object.


| Element | Description | Size (Bytes) |
| ------- | ----------- | ------------ |
| Name | The name of the mesh | 8 + (string length) |
| Vertex Count | How many Vertices there are in this mesh | 8 |
| Face Count | How many Faces there are in this mesh | 8 |
| Positions | Vertex Positions (XYZ) | 12 * (Vertex Count) |  
| UV Channels | How many UV Channels are in use by this mesh | 8 |
| TexCoords Count | How many texure coordinates are used by this UV channel - Written once per UV channel | 8 |
| TexCoords | Texture Coordinates (XY) - Written once per UV Channel | 8 * (TexCoords Count) |  
| Normal Count | How many Normals are used by this mesh | 8 |
| Normals | Vertex Normals (XYZ) | 12 * (Normal Count) |  
| Tangent Count | How many Tangents are used by this mesh | 8 |
| Tangent | Vertex Tangents (XYZ) | 12 * (Tangent Count) |  
| Binormal Count | How many Binormals are used by this mesh | 8 |
| Binormals | Vertex Binormals (XYZ) | 12 * (Binormal Count) |  
| Indices | Triangulated Mesh Indices (unsigned) | 4 * (Face Count) * 3 |


