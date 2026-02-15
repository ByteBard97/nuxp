/**
 * NUXP SDK Client - Auto-generated index
 * Re-exports all generated suite modules
 */

export * from './AIAppContextSuite';
export * from './AIArtSetSuite';
export { NewArt, DisposeArt, ReorderArt, DuplicateArt, GetFirstArtOfLayer, GetLayerOfArt, GetArtType, GetArtUserAttr, SetArtUserAttr, GetArtParent, GetArtFirstChild, GetArtSibling, GetArtPriorSibling, GetArtBounds, SetArtBounds, GetArtCenterPointVisible, SetArtCenterPointVisible, GetArtTransformBounds, UpdateArtworkLink, ValidArt, GetArtOrder, SelectNamedArtOfLayer, GetArtRotatedBounds, ArtHasFill, ArtHasStroke, ArtCopyFillStyleIfEqualPaths, ArtCopyStrokeStyleIfEqualPaths, GetInsertionPoint, SetInsertionPoint, GetKeyArt, HasDictionary, IsDictionaryEmpty, SetArtName, GetArtName, IsArtLayerGroup, ReleaseToLayers, ModifyTargetedArtSet, IsArtStyledArt, IsArtClipping, TransferAttributes, GetArtLastChild, SetArtTextWrapProperty, GetArtTextWrapProperty, CreateCopyScope, DestroyCopyScope, InsertionPointBadForArtType, PreinsertionFlightCheck, SetNote, GetNote, HasNote, GetArtXMPSize, SetArtXMP, GetPreciseArtTransformBounds, UncheckedDisposeArt, ArtIsGraph, SetKeyArt, GetDrawingMode, SetDrawingMode, GetInsertionPointForDrawingMode, GetInsertionPointForCurrentDrawingMode, GetPathPolarity, IsPixelPerfect, SetPixelPerfect, IsArtALayerInSymbol, GetArtTimeStamp, ConvertPointTypeToAreaType, ConvertAreaTypeToPointType, MarkDirty, GetSafeArtHandle, GetArtHandle, GetArtDefaultName, GetDocumentOfArt } from './AIArtSuite';
export * from './AIArtboardSuite';
export { GetBlendingMode, GetOpacity, SetOpacity, GetIsolated, SetIsolated, GetKnockout, GetInheritedKnockout, GetAlphaIsShape, SetAlphaIsShape, Copy, GetArtAttrs, SetArtAttrs, GetStyleAttrs, SetStyleAttrs, GetCurrentTransparency, SetCurrentTransparency, GetFocalFillAttrs, GetFocalStrokeAttrs, SetFocalFillAttrs, SetFocalStrokeAttrs, ContainsNonIsolatedBlending, GetDocumentIsolated, SetDocumentIsolated, GetDocumentKnockout, SetDocumentKnockout } from './AIBlendStyleSuite';
export { CreateDictionary, CreateDictionaryFromJSONFile, Clone, Begin, DeleteEntry, GetEntryType, CopyEntry, MoveEntry, SwapEntries, GetArtEntry, NewArtEntry, MoveArtToEntry, MoveEntryToArt, CopyArtToEntry, CopyEntryToArt, SetEntryToLayer, SetLayerToEntry, Set, GetBooleanEntry, SetBooleanEntry, GetIntegerEntry, SetIntegerEntry, GetPointerEntry, SetPointerEntry, GetRealEntry, SetRealEntry, SetStringEntry, GetDictEntry, SetDictEntry, GetArrayEntry, SetArrayEntry, GetUnicodeStringEntry, SetUnicodeStringEntry, TouchArt, Find } from './AIDictionarySuite';
export { GetDocumentFileSpecification, GetDocumentFileSpecificationFromHandle, GetDocumentPageOrigin, SetDocumentPageOrigin, SetDocumentRulerOrigin, GetDocumentRulerUnits, SetDocumentRulerUnits, GetDocumentCropStyle, SetDocumentCropStyle, GetDocumentPrintRecord, SetDocumentPrintRecord, GetDocumentSetup, SetDocumentSetup, GetDocumentModified, SetDocumentModified, GetDocumentFileFormat, SetDocumentFileFormat, SetDocumentFileFormatParameters, GetDocument, WriteDocument, GetDocumentMiPrintRecord, SetDocumentMiPrintRecord, GetDocumentRulerOrigin, UpdateLinks, GetDocumentZoomLimit, GetDocumentMaxArtboardBounds, DocumentExists, GetDocumentColorModel, SetDocumentColorModel, Cut, Paste, GetDocumentTargeting, SetDocumentTargeting, DocumentHasTransparency, DocumentHasSpotColorArt, SetDocumentAssetMgmtInfo, SetDocumentXAP, SuspendTextReflow, ResumeTextReflow, HasTextFocus, HasTextCaret, GetTextFocus, SetTextFocus, LoseTextFocus, GetDocumentTextResources, WriteDocumentMacInformationResource, WriteDocumentAsLibrary, DocumentHasOverprint, DocumentHasManagedLinks, GetDocumentSpotColorMode, Undo, Redo, DocumentRasterAttributes, GetDocumentStartupProfile, GetDocumentBleeds, SetDocumentBleeds, SetDocumentPixelPerfectStatus, DeleteSelection, SetAutoAssignUIDOnArtCreation, GetAutoAssignUIDOnArtCreation, GetDocumentScale, GetDocumentFileName, GetDocumentFileNameNoExt, GetDocumentFileNameFromHandle, GetDocumentFileNameNoExtFromHandle, GetLastExportedFilePath, SetLastExportedFilePath } from './AIDocumentSuite';
export * from './AIDocumentViewSuite';
export * from './AIEntrySuite';
export * from './AIGroupSuite';
export { Count, GetFirst, GetLast, GetNext, CountLayers, GetFirstLayer, GetLastLayer, GetNextLayer, GetPrevLayer, SetDisplayMode, SetEditabilityMode } from './AILayerListSuite';
export { GetNthLayer, GetCurrentLayer, SetCurrentLayer, InsertLayer, DeleteLayer, GetLayerTitle, SetLayerTitle, GetLayerColor, GetLayerVisible, SetLayerVisible, GetLayerPreview, SetLayerPreview, GetLayerEditable, SetLayerEditable, GetLayerPrinted, SetLayerPrinted, GetLayerDimPlacedImages, SetLayerDimPlacedImages, GetLayerSelected, SetLayerSelected, GetLayerByTitle, LayerHasArt, LayerHasSelectedArt, DeselectArtOnLayer, SelectArtOnLayer, GetLayerIsTemplate, SetLayerIsTemplate, GetLayerDimmedPercent, SetLayerDimmedPercent, GetLayerFirstChild, GetLayerParent, InsertLayerAtArt, ChangeLayerToGroup, GetNextPreorderLayer, GetNextNonChildPreorderLayer } from './AILayerSuite';
export { GetMask, CreateMask, DeleteMask, GetLinked, SetLinked, GetDisabled, SetDisabled, GetInverted, SetInverted, GetArt, IsEditingArt, SetEditingArt, GetMaskedArt, GetClipping, SetClipping } from './AIMaskSuite';
export * from './AIMdMemorySuite';
export * from './AINotifierSuite';
export * from './AITimerSuite';
export * from './AIToolSuite';
export * from './AITransformArtSuite';
export * from './AIUndoSuite';
export * from './AIUserSuite';
export * from './customRoutes';
export * from './events';
export * from './types';
