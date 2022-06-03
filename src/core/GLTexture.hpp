#pragma once
#include <glad/glad.h>

#include <GLDefinitions.hpp>
#include <GLObject.hpp>
#include <GLObjectBinder.hpp>
#include <GLTextureUnit.hpp>
#include <vector>
#include <array>
namespace dream
{
namespace glwrapper
{
template <GLenum TextureType, GLenum TextureBindingType>
class GLTexture : public GLObject
{
public:
	GLTexture(const std::string &name = "GLTexture")
		: GLObject(name)
	{
		glGenTextures(1, &mObjectId);
	}

	~GLTexture() override
	{
		glDeleteTextures(1, &mObjectId);
	}

	enum InternalFormat
	{
		Depth						   = GL_DEPTH_COMPONENT,
		DepthStencil				   = GL_DEPTH_STENCIL,
		Red							   = GL_RED,
		Rg							   = GL_RG,
		Rgb							   = GL_RGB,
		Rgba						   = GL_RGBA,
		R8							   = GL_R8,
		R8_snorm					   = GL_R8_SNORM,
		R16							   = GL_R16,
		R16_snorm					   = GL_R16_SNORM,
		Rg8							   = GL_RG8,
		Rg8_snorm					   = GL_RG8_SNORM,
		Rg16						   = GL_RG16,
		Rg16_snorm					   = GL_RG16_SNORM,
		R3_g3_b2					   = GL_R3_G3_B2,
		Rgb4						   = GL_RGB4,
		Rgb5						   = GL_RGB5,
		Rgb8						   = GL_RGB8,
		Rgb8_snorm					   = GL_RGB8_SNORM,
		Rgb10						   = GL_RGB10,
		Rgb12						   = GL_RGB12,
		Rgb16_snorm					   = GL_RGB16_SNORM,
		Rgba2						   = GL_RGBA2,
		Rgba4						   = GL_RGBA4,
		Rgb5_a1						   = GL_RGB5_A1,
		Rgba8						   = GL_RGBA8,
		Rgba8_snorm					   = GL_RGBA8_SNORM,
		Rgb10_a2					   = GL_RGB10_A2,
		Rgb10_a2ui					   = GL_RGB10_A2UI,
		Rgba12						   = GL_RGBA12,
		Rgba16						   = GL_RGBA16,
		Srgb8						   = GL_SRGB8,
		Srgb8_alpha8				   = GL_SRGB8_ALPHA8,
		R16f						   = GL_R16F,
		Rg16f						   = GL_RG16F,
		Rgb16f						   = GL_RGB16F,
		Rgba16f						   = GL_RGBA16F,
		R32f						   = GL_R32F,
		Rg32f						   = GL_RG32F,
		Rgb32f						   = GL_RGB32F,
		Rgba32f						   = GL_RGBA32F,
		R11f_g11f_b10f				   = GL_R11F_G11F_B10F,
		Rgb9_e5						   = GL_RGB9_E5,
		R8i							   = GL_R8I,
		R8ui						   = GL_R8UI,
		R16i						   = GL_R16I,
		R16ui						   = GL_R16UI,
		R32i						   = GL_R32I,
		R32ui						   = GL_R32UI,
		Rg8i						   = GL_RG8I,
		Rg8ui						   = GL_RG8UI,
		Rg16i						   = GL_RG16I,
		Rg16ui						   = GL_RG16UI,
		Rg32i						   = GL_RG32I,
		Rg32ui						   = GL_RG32UI,
		Rgb8i						   = GL_RGB8I,
		Rgb8ui						   = GL_RGB8UI,
		Rgb16i						   = GL_RGB16I,
		Rgb16ui						   = GL_RGB16UI,
		Rgb32i						   = GL_RGB32I,
		Rgb32ui						   = GL_RGB32UI,
		Rgba8i						   = GL_RGBA8I,
		Rgba8ui						   = GL_RGBA8UI,
		Rgba16i						   = GL_RGBA16I,
		Rgba16ui					   = GL_RGBA16UI,
		Rgba32i						   = GL_RGBA32I,
		Rgba32ui					   = GL_RGBA32UI,
		CompressedRed				   = GL_COMPRESSED_RED,
		CompressedRg				   = GL_COMPRESSED_RG,
		CompressedRgb				   = GL_COMPRESSED_RGB,
		CompressedRgba				   = GL_COMPRESSED_RGBA,
		CompressedSrgb				   = GL_COMPRESSED_SRGB,
		CompressedSrgb_alpha		   = GL_COMPRESSED_SRGB_ALPHA,
		CompressedRedRgtc1			   = GL_COMPRESSED_RED_RGTC1,
		CompressedSigned_red_rgtc1	   = GL_COMPRESSED_SIGNED_RED_RGTC1,
		CompressedRgRgtc2			   = GL_COMPRESSED_RG_RGTC2,
		CompressedSignedRgRgtc2		   = GL_COMPRESSED_SIGNED_RG_RGTC2,
		CompressedRgbaBptcUnorm		   = GL_COMPRESSED_RGBA_BPTC_UNORM,
		CompressedSrgbAlphaBptcUnorm   = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
		CompressedRgbBptcSignedFloat   = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
		CompressedRgbBptcUnsignedFloat = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT,
	};

	enum DataFormat
	{
		DRed			= GL_RED,
		DRg				= GL_RG,
		DRgb			= GL_RGB,
		DBgr			= GL_BGR,
		DRgba			= GL_RGBA,
		DBgra			= GL_BGRA,
		DRedInteger		= GL_RED_INTEGER,
		DRgInteger		= GL_RG_INTEGER,
		DRgbInteger		= GL_RGB_INTEGER,
		DBgrInteger		= GL_BGR_INTEGER,
		DRgbaInteger	= GL_RGBA_INTEGER,
		DBgraInteger	= GL_BGRA_INTEGER,
		DStencilIndex	= GL_STENCIL_INDEX,
		DDepthComponent = GL_DEPTH_COMPONENT,
		DDepthStencil	= GL_DEPTH_STENCIL
	};

	enum DataType
	{
		UByte				= GL_UNSIGNED_BYTE,
		Byte				= GL_BYTE,
		UShort				= GL_UNSIGNED_SHORT,
		Short				= GL_SHORT,
		UInt				= GL_UNSIGNED_INT,
		Int					= GL_INT,
		HFloat				= GL_HALF_FLOAT,
		Float				= GL_FLOAT,
		UByte_3_3_2			= GL_UNSIGNED_BYTE_3_3_2,
		UByte_2_3_3_rev		= GL_UNSIGNED_BYTE_2_3_3_REV,
		UShort_5_6_5		= GL_UNSIGNED_SHORT_5_6_5,
		UShort_5_6_5_rev	= GL_UNSIGNED_SHORT_5_6_5_REV,
		UShort_4_4_4_4		= GL_UNSIGNED_SHORT_4_4_4_4,
		UShort_4_4_4_4_rev	= GL_UNSIGNED_SHORT_4_4_4_4_REV,
		UShort_5_5_5_1		= GL_UNSIGNED_SHORT_5_5_5_1,
		UShort_1_5_5_5_rev	= GL_UNSIGNED_SHORT_1_5_5_5_REV,
		UInt_8_8_8_8		= GL_UNSIGNED_INT_8_8_8_8,
		UInt_8_8_8_8_rev	= GL_UNSIGNED_INT_8_8_8_8_REV,
		UInt_10_10_10_2		= GL_UNSIGNED_INT_10_10_10_2,
		UInt_2_10_10_10_rev = GL_UNSIGNED_INT_2_10_10_10_REV
	};

	enum TextureParameters
	{
		DepthStencilTextureMode = GL_DEPTH_STENCIL_TEXTURE_MODE,
		TextureBaseLevel		= GL_TEXTURE_BASE_LEVEL,
		TextureCompareFunc		= GL_TEXTURE_COMPARE_FUNC,
		TextureCompareMode		= GL_TEXTURE_COMPARE_MODE,
		TextureLodBias			= GL_TEXTURE_LOD_BIAS,
		TextureMinFilter		= GL_TEXTURE_MIN_FILTER,
		TextureMagFilter		= GL_TEXTURE_MAG_FILTER,
		TextureMinLod			= GL_TEXTURE_MIN_LOD,
		TextureMaxLod			= GL_TEXTURE_MAX_LOD,
		TextureMaxLevel			= GL_TEXTURE_MAX_LEVEL,
		TextureSwizzleR			= GL_TEXTURE_SWIZZLE_R,
		TextureSwizzleG			= GL_TEXTURE_SWIZZLE_G,
		TextureSwizzleB			= GL_TEXTURE_SWIZZLE_B,
		TextureSwizzleA			= GL_TEXTURE_SWIZZLE_A,
		TextureWrapS			= GL_TEXTURE_WRAP_S,
		TextureWrapT			= GL_TEXTURE_WRAP_T,
		TextureWrapR			= GL_TEXTURE_WRAP_R
	};

	enum ValueWrapS
	{
		ClampToEdgeS	   = GL_CLAMP_TO_EDGE,
		ClampToBorderS	   = GL_CLAMP_TO_BORDER,
		MirroredRepeatS	   = GL_MIRRORED_REPEAT,
		RepeatS			   = GL_REPEAT,
		MirrorClampToEdgeS = GL_MIRROR_CLAMP_TO_EDGE
	};

	enum ValueWrapT
	{
		ClampToEdgeT	   = GL_CLAMP_TO_EDGE,
		ClampToBorderT	   = GL_CLAMP_TO_BORDER,
		MirroredRepeatT	   = GL_MIRRORED_REPEAT,
		RepeatT			   = GL_REPEAT,
		MirrorClampToEdgeT = GL_MIRROR_CLAMP_TO_EDGE
	};

	enum ValueMinFilter
	{
		MinNearest				= GL_NEAREST,
		MinLinear				= GL_LINEAR,
		MinNearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
		MinLinearMipmapNearest	= GL_LINEAR_MIPMAP_NEAREST,
		MinNearestMipmapLinear	= GL_NEAREST_MIPMAP_LINEAR,
		MinLinearMipmapLinear	= GL_LINEAR_MIPMAP_LINEAR,
	};

	enum ValueMagFilter
	{
		MagLinear  = GL_LINEAR,
		MagNearest = GL_NEAREST,
	};

	enum VlueCompareFunction
	{
		Lequal	 = GL_LEQUAL,
		Gequal	 = GL_GEQUAL,
		Less	 = GL_LESS,
		Greater	 = GL_GREATER,
		Equal	 = GL_EQUAL,
		Notequal = GL_NOTEQUAL,
		Always	 = GL_ALWAYS,
		Never	 = GL_NEVER,
	};

	/// Create mutable storage for the buffer of specified type and deploy it
	/// with data in buff
	template <class T, int Dimentions>
	void create(std::vector<T> &buff, GLint level, InternalFormat intForm,
				DataFormat dataFormat, DataType dataType,
				const std::array<std::size_t, Dimentions> &dim)
	{
		static_assert(Dimentions > 0 && Dimentions <= 3,
					  "Dimentionality error");
		GLObjectBinder bind(*this);
		if (Dimentions == 1)
			glTexImage1D(TextureType, level, intForm, dim[0], 0, dataFormat,
						 dataType, reinterpret_cast<const void *>(buff.data()));
		if (Dimentions == 2)
			glTexImage2D(TextureType, level, intForm, dim[0], dim[1], 0,
						 dataFormat, dataType,
						 reinterpret_cast<const void *>(buff.data()));
		if (Dimentions == 3)
			glTexImage3D(TextureType, level, intForm, dim[0], dim[1], dim[2], 0,
						 dataFormat, dataType,
						 reinterpret_cast<const void *>(buff.data()));
	}

	/// create mutable storage for buffer without storing data there
	template <int Dimentions>
	void create(GLint level, InternalFormat intForm,
				const std::array<std::size_t, Dimentions> &dim)
	{
		static_assert(Dimentions > 0 && Dimentions <= 3,
					  "Dimentionality error");
		GLObjectBinder bind(*this);
		if (Dimentions == 1)
			glTexImage1D(TextureType, level, intForm, dim[0], 0,
						 DataFormat::DRed, DataType::UByte, nullptr);
		if (Dimentions == 2)
			glTexImage2D(TextureType, level, intForm, dim[0], dim[1], 0,
						 DataFormat::DRed, DataType::UByte, nullptr);
		if (Dimentions == 3)
			glTexImage3D(TextureType, level, intForm, dim[0], dim[1], dim[2], 0,
						 DataFormat::DRed, DataType::UByte, nullptr);
	}

	/// Create immutable storage for the buffer
	/** After creation buffer size should not be changed
	 * thus all operations are performed on the same location
	 */
	template <class T>
	void createImmutable(std::vector<T> &buff, GLenum type)
	{
	}

	/// Create immutable storage without initialisation
	void createImmutable(size_t size, GLenum type)
	{
	}

	void attach(GLenum unit)
	{
		char bufTexBinder[sizeof(GLObjectBinder)];
		char bufUnitBinder[sizeof(GLObjectBinder)];

		GLTextureUnit texUnit(unit);

		// bind texture after texture unit
		// and unbind in same order to preserve texture attachment to this
		// unit
		GLObjectBinder *textureUnitBinder = new (bufUnitBinder)
			GLObjectBinder(texUnit);
		GLObjectBinder *textureBinder = new (bufTexBinder)
			GLObjectBinder(*this);

		textureUnitBinder->~GLObjectBinder();
		textureBinder->~GLObjectBinder();
	}
#define PARAMETER_SETTER(Parameter, ParamValueType)                            \
	void setParameter(ParamValueType val)                                      \
	{                                                                          \
		GLObjectBinder bind(*this);                                            \
		glTexParameteri(TextureType, Parameter, val);                          \
	}

	PARAMETER_SETTER(TextureParameters::TextureWrapS, ValueWrapS)
	PARAMETER_SETTER(TextureParameters::TextureWrapT, ValueWrapT)
	PARAMETER_SETTER(TextureParameters::TextureMinFilter, ValueMinFilter)
	PARAMETER_SETTER(TextureParameters::TextureMagFilter, ValueMagFilter)
	PARAMETER_SETTER(TextureParameters::TextureCompareFunc, VlueCompareFunction)

	template <TextureParameters Param>
	GLint getParameter()
	{
		GLObjectBinder bind(*this);
		GLint val;
		glGetTexParameteriv(TextureType, static_cast<GLenum>(Param), &val);
		return val;
	}

	void generateMipMaps()
	{
		GLObjectBinder bind(*this);
		glGenerateMipmap(TextureType);
	}

protected:
	void bind() override
	{
		mObjectsBeforeBinding.push_back(0);
		glGetIntegerv(TextureBindingType, &mObjectsBeforeBinding.back());
		glBindTexture(TextureType, mObjectId);
	}

	void unbind() override
	{
		glBindTexture(TextureType, mObjectsBeforeBinding.back());
		mObjectsBeforeBinding.pop_back();
	}
};

using GLTexture2D = GLTexture<GL_TEXTURE_2D, GL_TEXTURE_BINDING_2D>;

} // namespace glwrapper
} // namespace dream
