// GENERATED FILE - DO NOT EDIT.
// Generated by gen_features.py using data from frontend_features.json.
//
// Copyright 2022 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// FrontendFeatures_autogen.h: Features/workarounds for driver bugs and other behaviors seen
// on all platforms.

#ifndef ANGLE_PLATFORM_AUTOGEN_FRONTENDFEATURES_H_
#define ANGLE_PLATFORM_AUTOGEN_FRONTENDFEATURES_H_

#include "platform/Feature.h"

namespace angle
{

struct FrontendFeatures : FeatureSetBase
{
    FrontendFeatures();
    ~FrontendFeatures();

    FeatureInfo loseContextOnOutOfMemory = {
        "loseContextOnOutOfMemory",
        FeatureCategory::FrontendWorkarounds,
        &members,
    };

    FeatureInfo disableProgramCachingForTransformFeedback = {
        "disableProgramCachingForTransformFeedback",
        FeatureCategory::FrontendWorkarounds,
        &members,
    };

    FeatureInfo disableProgramBinary = {
        "disableProgramBinary",
        FeatureCategory::FrontendFeatures,
        &members,
    };

    FeatureInfo disableDrawBuffersIndexed = {
        "disableDrawBuffersIndexed",
        FeatureCategory::FrontendFeatures,
        &members,
    };

    FeatureInfo disableAnisotropicFiltering = {
        "disableAnisotropicFiltering",
        FeatureCategory::FrontendWorkarounds,
        &members,
    };

    FeatureInfo allowCompressedFormats = {
        "allowCompressedFormats",
        FeatureCategory::FrontendWorkarounds,
        &members,
    };

    FeatureInfo singleThreadedTextureDecompression = {
        "singleThreadedTextureDecompression",
        FeatureCategory::FrontendWorkarounds,
        &members,
    };

    FeatureInfo forceDepthAttachmentInitOnClear = {
        "forceDepthAttachmentInitOnClear",
        FeatureCategory::FrontendWorkarounds,
        &members,
    };

    FeatureInfo enableCaptureLimits = {
        "enableCaptureLimits",
        FeatureCategory::FrontendFeatures,
        &members,
    };

    FeatureInfo forceRobustResourceInit = {
        "forceRobustResourceInit",
        FeatureCategory::FrontendFeatures,
        &members,
    };

    FeatureInfo forceInitShaderVariables = {
        "forceInitShaderVariables",
        FeatureCategory::FrontendFeatures,
        &members,
    };

    FeatureInfo enableProgramBinaryForCapture = {
        "enableProgramBinaryForCapture",
        FeatureCategory::FrontendFeatures,
        &members,
    };

    FeatureInfo forceGlErrorChecking = {
        "forceGlErrorChecking",
        FeatureCategory::FrontendFeatures,
        &members,
    };

    FeatureInfo emulatePixelLocalStorage = {
        "emulatePixelLocalStorage",
        FeatureCategory::FrontendFeatures,
        &members,
    };

    FeatureInfo cacheCompiledShader = {
        "cacheCompiledShader",
        FeatureCategory::FrontendFeatures,
        &members,
    };

    FeatureInfo dumpShaderSource = {
        "dumpShaderSource",
        FeatureCategory::FrontendFeatures,
        &members,
    };

    FeatureInfo enableShaderSubstitution = {
        "enableShaderSubstitution",
        FeatureCategory::FrontendWorkarounds,
        &members,
    };

    FeatureInfo disableProgramCaching = {
        "disableProgramCaching",
        FeatureCategory::FrontendFeatures,
        &members,
    };

    FeatureInfo dumpTranslatedShaders = {
        "dumpTranslatedShaders",
        FeatureCategory::FrontendFeatures,
        &members,
    };

    FeatureInfo enableTranslatedShaderSubstitution = {
        "enableTranslatedShaderSubstitution",
        FeatureCategory::FrontendWorkarounds,
        &members,
    };

    FeatureInfo compileJobIsThreadSafe = {
        "compileJobIsThreadSafe",
        FeatureCategory::FrontendFeatures,
        &members,
    };

    FeatureInfo linkJobIsThreadSafe = {
        "linkJobIsThreadSafe",
        FeatureCategory::FrontendFeatures,
        &members,
    };

    FeatureInfo alwaysRunLinkSubJobsThreaded = {
        "alwaysRunLinkSubJobsThreaded",
        FeatureCategory::FrontendFeatures,
        &members,
    };

    FeatureInfo uncurrentEglSurfaceUponSurfaceDestroy = {
        "uncurrentEglSurfaceUponSurfaceDestroy",
        FeatureCategory::FrontendWorkarounds,
        &members,
    };

    FeatureInfo forceMinimumMaxVertexAttributes = {
        "forceMinimumMaxVertexAttributes",
        FeatureCategory::FrontendFeatures,
        &members,
    };

    FeatureInfo forceFlushAfterDrawcallUsingShadowmap = {
        "forceFlushAfterDrawcallUsingShadowmap",
        FeatureCategory::FrontendWorkarounds,
        &members,
    };

    FeatureInfo rejectWebglShadersWithUndefinedBehavior = {
        "rejectWebglShadersWithUndefinedBehavior",
        FeatureCategory::FrontendWorkarounds,
        &members,
    };

    FeatureInfo alwaysEnableEmulatedMultidrawExtensions = {
        "alwaysEnableEmulatedMultidrawExtensions",
        FeatureCategory::FrontendWorkarounds,
        &members,
    };

};

inline FrontendFeatures::FrontendFeatures()  = default;
inline FrontendFeatures::~FrontendFeatures() = default;

}  // namespace angle

#endif  // ANGLE_PLATFORM_AUTOGEN_FRONTENDFEATURES_H_