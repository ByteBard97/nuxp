import type { Script } from '../types'
import { logger, createLogger } from '@nuxp/sdk'

const sdkLogger: Script = {
  id: 'sdk-logger',
  name: 'LoggerService',
  description: 'Per-module logging with ring buffer, level filtering, and module enable/disable',
  category: 'sdk',
  async run() {
    // Create module-scoped loggers
    const bridgeLog = createLogger('bridge')
    const syncLog = createLogger('sync')
    const uiLog = createLogger('ui')

    // Log at different levels from different modules
    bridgeLog.info('Bridge connected to plugin on port 8081')
    bridgeLog.debug('Sending callSuite request', { suite: 'AIDocumentSuite', fn: 'GetDocumentTitle' })
    syncLog.warn('Sync retry #3 — server did not acknowledge')
    uiLog.error('Failed to render component', { component: 'ScriptCard', reason: 'missing prop' })
    uiLog.info('UI initialized successfully')

    // Show registered modules
    const modules = logger.getRegisteredModules()
    const moduleStates = logger.getModuleStates()

    // Get recent log entries from the ring buffer
    const allLogs = logger.getLogs()
    const recentLogs = allLogs.slice(-10).map(entry => ({
      time: entry.time,
      level: entry.type,
      module: entry.module || '(global)',
      message: entry.message,
    }))

    return {
      success: true,
      message: `Logger has ${allLogs.length} entries across ${modules.length} modules`,
      data: {
        globalLevel: logger.getGlobalLevel(),
        registeredModules: modules,
        moduleStates,
        recentLogs,
      },
    }
  },
}

export default sdkLogger
