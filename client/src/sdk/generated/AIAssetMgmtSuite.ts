/**
 * AIAssetMgmtSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AIAssetMgmtSuite';

/**
 * Updates the object state.
 * @param mimeType - The Mime Type value
 * @param managedURL - The Managed U R L value
 * @param metadataSelectors - The selection state
 */
export async function UpdateDynamicMetadata(mimeType: any, managedURL: any, metadataSelectors: any): Promise<void> {
    await callCpp(SUITE_NAME, 'UpdateDynamicMetadata', { mimeType, managedURL, metadataSelectors });
}

/**
 * Shows the object.
 * @param xmpIn - The Xmp In value
 * @param dialogTitle - The Dialog Title value
 * @returns The xmp out value
 */
export async function ShowMetadataDialog(xmpIn: any, dialogTitle: any): Promise<any> {
    const result = await callCpp<{ xmpOut: any }>(SUITE_NAME, 'ShowMetadataDialog', { xmpIn, dialogTitle });
    return result.xmpOut;
}
