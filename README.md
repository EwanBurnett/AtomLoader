# Atom Loader
A utility to convert 3D model files into a binary format, and a library to load the binary into a convenient struct.

Supports all formats used by [Assimp](https://github.com/assimp/assimp/blob/master/doc/Fileformats.md)

## Binary Format
### Metadata
| Element | Description | Size (Bytes) |
| ------- | ----------- | ------------ |
| Version | The version of Atom used to serialize the file. | 4 |
| Source File | The filename of the model's source. | 8 + n |
| Hash ID | The SHA-256 Hashed ID of the model. | 32 |
| Mesh Count | How many meshes are in this model. | 8 |

### Mesh Data
| Element | Description | Size (Bytes) |
| ------- | ----------- | ------------ |
| Name | The name of the mesh | 8 + n |
| Vertex Count | How many Vertices there are in this mesh | 8 |
| Face Count | How many Faces there are in this mesh | 8 |
| Positions | Vertex Positions (XYZ) | 12 * n |  
| TexCoords | Texture Coordinates (XY) | 8 * n |  
| Normals | Vertex Normals (XYZ) | 12 * n |  
| Tangent | Vertex Tangents (XYZ) | 12 * n |  
| Binormals | Vertex Binormals (XYZ) | 12 * n |  
| Indices | Mesh Indices (unsigned) | 4 * n |

## Usage 

```
Model cube;
cube
```