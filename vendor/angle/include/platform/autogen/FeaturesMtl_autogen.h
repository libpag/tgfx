// GENERATED FILE - DO NOT EDIT.
// Generated by gen_features.py using data from mtl_features.json.
//
// Copyright 2022 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// FeaturesMtl_autogen.h: Optional features for the Metal renderer.

#ifndef ANGLE_PLATFORM_AUTOGEN_FEATURESMTL_H_
#define ANGLE_PLATFORM_AUTOGEN_FEATURESMTL_H_

#include "platform/Feature.h"

namespace angle
{

struct FeaturesMtl : FeatureSetBase
{
    FeaturesMtl();
    ~FeaturesMtl();

    FeatureInfo hasBaseVertexInstancedDraw = {
        "hasBaseVertexInstancedDraw",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo hasExplicitMemBarrier = {
        "hasExplicitMemBarrier",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo hasCheapRenderPass = {
        "hasCheapRenderPass",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo hasNonUniformDispatch = {
        "hasNonUniformDispatch",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo hasShaderStencilOutput = {
        "hasShaderStencilOutput",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo hasTextureSwizzle = {
        "hasTextureSwizzle",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo hasDepthAutoResolve = {
        "hasDepthAutoResolve",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo hasStencilAutoResolve = {
        "hasStencilAutoResolve",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo hasEvents = {
        "hasEvents",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo allowInlineConstVertexData = {
        "allowInlineConstVertexData",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo allowSeparateDepthStencilBuffers = {
        "allowSeparateDepthStencilBuffers",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo allowRuntimeSamplerCompareMode = {
        "allowRuntimeSamplerCompareMode",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo allowSamplerCompareGradient = {
        "allowSamplerCompareGradient",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo allowBufferReadWrite = {
        "allowBufferReadWrite",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo allowMultisampleStoreAndResolve = {
        "allowMultisampleStoreAndResolve",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo allowGenMultipleMipsPerPass = {
        "allowGenMultipleMipsPerPass",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo forceD24S8AsUnsupported = {
        "forceD24S8AsUnsupported",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo forceBufferGPUStorage = {
        "forceBufferGPUStorage",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo forceNonCSBaseMipmapGeneration = {
        "forceNonCSBaseMipmapGeneration",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo emulateTransformFeedback = {
        "emulateTransformFeedback",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo intelExplicitBoolCastWorkaround = {
        "intelExplicitBoolCastWorkaround",
        FeatureCategory::MetalWorkarounds,
        &members,
    };

    FeatureInfo intelDisableFastMath = {
        "intelDisableFastMath",
        FeatureCategory::MetalWorkarounds,
        &members,
    };

    FeatureInfo allowRenderpassWithoutAttachment = {
        "allowRenderpassWithoutAttachment",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo avoidStencilTextureSwizzle = {
        "avoidStencilTextureSwizzle",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo emulateAlphaToCoverage = {
        "emulateAlphaToCoverage",
        FeatureCategory::MetalWorkarounds,
        &members,
    };

    FeatureInfo writeHelperSampleMask = {
        "writeHelperSampleMask",
        FeatureCategory::MetalWorkarounds,
        &members,
    };

    FeatureInfo preTransformTextureCubeGradDerivatives = {
        "preTransformTextureCubeGradDerivatives",
        FeatureCategory::MetalWorkarounds,
        &members,
    };

    FeatureInfo multisampleColorFormatShaderReadWorkaround = {
        "multisampleColorFormatShaderReadWorkaround",
        FeatureCategory::MetalWorkarounds,
        &members,
    };

    FeatureInfo copyIOSurfaceToNonIOSurfaceForReadOptimization = {
        "copyIOSurfaceToNonIOSurfaceForReadOptimization",
        FeatureCategory::MetalWorkarounds,
        &members,
    };

    FeatureInfo copyTextureToBufferForReadOptimization = {
        "copyTextureToBufferForReadOptimization",
        FeatureCategory::MetalWorkarounds,
        &members,
    };

    FeatureInfo limitMaxDrawBuffersForTesting = {
        "limitMaxDrawBuffersForTesting",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo limitMaxColorTargetBitsForTesting = {
        "limitMaxColorTargetBitsForTesting",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo preemptivelyStartProvokingVertexCommandBuffer = {
        "preemptivelyStartProvokingVertexCommandBuffer",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo uploadDataToIosurfacesWithStagingBuffers = {
        "uploadDataToIosurfacesWithStagingBuffers",
        FeatureCategory::MetalWorkarounds,
        &members,
    };

    FeatureInfo alwaysUseStagedBufferUpdates = {
        "alwaysUseStagedBufferUpdates",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo useShadowBuffersWhenAppropriate = {
        "useShadowBuffersWhenAppropriate",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo alwaysUseManagedStorageModeForBuffers = {
        "alwaysUseManagedStorageModeForBuffers",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo alwaysUseSharedStorageModeForBuffers = {
        "alwaysUseSharedStorageModeForBuffers",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo preferCpuForBuffersubdata = {
        "preferCpuForBuffersubdata",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo disableProgrammableBlending = {
        "disableProgrammableBlending",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo disableRWTextureTier2Support = {
        "disableRWTextureTier2Support",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo disableRasterOrderGroups = {
        "disableRasterOrderGroups",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo enableInMemoryMtlLibraryCache = {
        "enableInMemoryMtlLibraryCache",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo enableParallelMtlLibraryCompilation = {
        "enableParallelMtlLibraryCompilation",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo alwaysPreferStagedTextureUploads = {
        "alwaysPreferStagedTextureUploads",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo disableStagedInitializationOfPackedTextureFormats = {
        "disableStagedInitializationOfPackedTextureFormats",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo compileMetalShaders = {
        "compileMetalShaders",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo loadMetalShadersFromBlobCache = {
        "loadMetalShadersFromBlobCache",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo printMetalShaders = {
        "printMetalShaders",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo generateShareableShaders = {
        "generateShareableShaders",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo disableMetalOnNvidia = {
        "disableMetalOnNvidia",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo flushAfterStreamVertexData = {
        "flushAfterStreamVertexData",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo requireGpuFamily2 = {
        "requireGpuFamily2",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo rescopeGlobalVariables = {
        "rescopeGlobalVariables",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo enableMultisampledRenderToTextureOnNonTilers = {
        "enableMultisampledRenderToTextureOnNonTilers",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo emulateDontCareLoadWithRandomClear = {
        "emulateDontCareLoadWithRandomClear",
        FeatureCategory::MetalFeatures,
        &members,
    };

    FeatureInfo injectAsmStatementIntoLoopBodies = {
        "injectAsmStatementIntoLoopBodies",
        FeatureCategory::MetalFeatures,
        &members,
    };

};

inline FeaturesMtl::FeaturesMtl()  = default;
inline FeaturesMtl::~FeaturesMtl() = default;

}  // namespace angle

#endif  // ANGLE_PLATFORM_AUTOGEN_FEATURESMTL_H_