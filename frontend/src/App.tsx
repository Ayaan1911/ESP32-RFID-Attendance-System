import { useEffect, useMemo, useState } from 'react'
import type { FormEvent } from 'react'
import type { AttendanceEvent } from './types'

const API_BASE_URL = 'http://localhost:8000'
const ATTENDANCE_ENDPOINT = `${API_BASE_URL}/attendance`
const REFRESH_INTERVAL_MS = 10_000

type LoadMode = 'idle' | 'loading' | 'ready' | 'error'

function sortNewestFirst(events: AttendanceEvent[]) {
  return [...events].sort(
    (a, b) => new Date(b.received_at).getTime() - new Date(a.received_at).getTime(),
  )
}

function formatDateTime(value: string | null) {
  if (!value) return '-'
  const date = new Date(value)
  if (Number.isNaN(date.getTime())) return value
  return new Intl.DateTimeFormat(undefined, {
    dateStyle: 'medium',
    timeStyle: 'medium',
  }).format(date)
}

function isToday(value: string) {
  const date = new Date(value)
  if (Number.isNaN(date.getTime())) return false

  const now = new Date()
  return (
    date.getFullYear() === now.getFullYear() &&
    date.getMonth() === now.getMonth() &&
    date.getDate() === now.getDate()
  )
}

async function fetchAttendance(apiKey: string) {
  const response = await fetch(ATTENDANCE_ENDPOINT, {
    headers: {
      'X-API-Key': apiKey,
    },
  })

  if (response.status === 401) {
    throw new Error('Invalid API key')
  }

  if (!response.ok) {
    throw new Error(`Failed to load attendance (${response.status})`)
  }

  return (await response.json()) as AttendanceEvent[]
}

export default function App() {
  const [draftKey, setDraftKey] = useState('')
  const [apiKey, setApiKey] = useState<string | null>(null)
  const [events, setEvents] = useState<AttendanceEvent[]>([])
  const [loadMode, setLoadMode] = useState<LoadMode>('idle')
  const [error, setError] = useState<string | null>(null)
  const [statusLine, setStatusLine] = useState('Waiting for API key')

  const sortedEvents = useMemo(() => sortNewestFirst(events), [events])

  const uniqueUidsToday = useMemo(() => {
    const todayUids = new Set(
      sortedEvents.filter((event) => isToday(event.received_at)).map((event) => event.uid),
    )
    return todayUids.size
  }, [sortedEvents])

  async function loadAttendance(nextApiKey = apiKey) {
    if (!nextApiKey) return

    setLoadMode('loading')
    setError(null)
    setStatusLine('Fetching attendance log')

    try {
      const data = await fetchAttendance(nextApiKey)
      setEvents(data)
      setApiKey(nextApiKey)
      setLoadMode('ready')
      setStatusLine(`Loaded ${data.length} event${data.length === 1 ? '' : 's'}`)
    } catch (err) {
      const message = err instanceof Error ? err.message : 'Failed to load attendance'
      setError(message)
      setLoadMode('error')
      setStatusLine('Attention required')

      if (message === 'Invalid API key') {
        setApiKey(null)
        setDraftKey('')
      }
    }
  }

  function handleSubmit(event: FormEvent<HTMLFormElement>) {
    event.preventDefault()
    const trimmed = draftKey.trim()
    if (!trimmed) {
      setError('Enter an API key to continue')
      return
    }
    setDraftKey(trimmed)
    void loadAttendance(trimmed)
  }

  useEffect(() => {
    if (!apiKey) return

    void loadAttendance(apiKey)
    const timer = window.setInterval(() => {
      void loadAttendance(apiKey)
    }, REFRESH_INTERVAL_MS)

    return () => window.clearInterval(timer)
  }, [apiKey])

  const isAuthenticated = Boolean(apiKey)

  return (
    <div className="min-h-screen bg-[radial-gradient(circle_at_top,_rgba(59,130,246,0.18),_transparent_36%),linear-gradient(180deg,#020617_0%,#020617_42%,#0f172a_100%)] text-slate-100">
      <div className="mx-auto flex min-h-screen max-w-7xl flex-col px-4 py-6 sm:px-6 lg:px-8">
        <header className="mb-6 flex flex-col gap-4 rounded-3xl border border-slate-800/90 bg-slate-900/70 px-5 py-4 shadow-glow backdrop-blur xl:flex-row xl:items-center xl:justify-between">
          <div>
            <p className="font-mono text-xs uppercase tracking-[0.35em] text-cyan-300/80">
              ESP32 RFID Attendance System
            </p>
            <h1 className="mt-2 text-2xl font-semibold tracking-tight text-slate-50 sm:text-3xl">
              Live Attendance Dashboard
            </h1>
            <p className="mt-2 max-w-2xl text-sm text-slate-300">
              Read-only view of attendance events synced from the PostgreSQL-backed backend.
            </p>
          </div>

          <div className="grid gap-2 rounded-2xl border border-slate-800 bg-slate-950/70 px-4 py-3 font-mono text-xs text-slate-300 sm:grid-cols-3">
            <div>
              <div className="text-slate-500">API</div>
              <div className="break-all text-cyan-300">{API_BASE_URL}</div>
            </div>
            <div>
              <div className="text-slate-500">Status</div>
              <div className="text-emerald-300">{statusLine}</div>
            </div>
            <div>
              <div className="text-slate-500">Mode</div>
              <div className="text-slate-200">{isAuthenticated ? 'Authenticated' : 'Locked'}</div>
            </div>
          </div>
        </header>

        {!isAuthenticated ? (
          <main className="flex flex-1 items-center justify-center">
            <section className="w-full max-w-lg rounded-3xl border border-slate-800 bg-slate-900/80 p-6 shadow-glow backdrop-blur">
              <div className="mb-6">
                <p className="font-mono text-xs uppercase tracking-[0.3em] text-slate-400">
                  Session access
                </p>
                <h2 className="mt-2 text-2xl font-semibold text-slate-50">
                  Enter backend API key
                </h2>
                <p className="mt-2 text-sm text-slate-300">
                  The key stays in React state only and resets on page refresh.
                </p>
              </div>

              <form onSubmit={handleSubmit} className="space-y-4">
                <label className="block">
                  <span className="mb-2 block font-mono text-xs uppercase tracking-[0.25em] text-slate-500">
                    X-API-Key
                  </span>
                  <input
                    autoFocus
                    type="password"
                    value={draftKey}
                    onChange={(event) => {
                      setDraftKey(event.target.value)
                      setError(null)
                    }}
                    placeholder="Paste the shared key here"
                    className="w-full rounded-2xl border border-slate-700 bg-slate-950 px-4 py-3 font-mono text-sm text-slate-100 outline-none transition placeholder:text-slate-600 focus:border-cyan-400 focus:ring-2 focus:ring-cyan-400/20"
                  />
                </label>

                {error ? (
                  <div className="rounded-2xl border border-rose-500/40 bg-rose-500/10 px-4 py-3 text-sm text-rose-200">
                    {error}
                  </div>
                ) : null}

                <button
                  type="submit"
                  className="inline-flex w-full items-center justify-center rounded-2xl bg-cyan-500 px-4 py-3 font-mono text-sm font-semibold uppercase tracking-[0.25em] text-slate-950 transition hover:bg-cyan-400"
                >
                  Open Dashboard
                </button>
              </form>
            </section>
          </main>
        ) : (
          <main className="flex-1 space-y-5">
            <section className="grid gap-4 md:grid-cols-2 xl:grid-cols-4">
              <SummaryCard label="Total events" value={String(sortedEvents.length)} />
              <SummaryCard label="Unique UIDs today" value={String(uniqueUidsToday)} />
              <SummaryCard
                label="Latest sync"
                value={sortedEvents[0] ? formatDateTime(sortedEvents[0].received_at) : '-'}
              />
              <SummaryCard label="Refresh cadence" value="10 seconds" />
            </section>

            <section className="rounded-3xl border border-slate-800 bg-slate-900/80 shadow-glow">
              <div className="flex flex-col gap-4 border-b border-slate-800 px-5 py-4 lg:flex-row lg:items-center lg:justify-between">
                <div>
                  <p className="font-mono text-xs uppercase tracking-[0.3em] text-slate-500">
                    Attendance feed
                  </p>
                  <h2 className="mt-1 text-xl font-semibold text-slate-50">Events</h2>
                </div>
                <div className="flex items-center gap-3">
                  <button
                    type="button"
                    onClick={() => void loadAttendance(apiKey)}
                    disabled={loadMode === 'loading'}
                    className="rounded-2xl border border-cyan-400/30 bg-cyan-400/10 px-4 py-2 font-mono text-xs uppercase tracking-[0.25em] text-cyan-200 transition hover:bg-cyan-400/20 disabled:cursor-not-allowed disabled:opacity-60"
                  >
                    Refresh
                  </button>
                  <button
                    type="button"
                    onClick={() => {
                      setApiKey(null)
                      setDraftKey('')
                      setEvents([])
                      setError(null)
                      setLoadMode('idle')
                      setStatusLine('Waiting for API key')
                    }}
                    className="rounded-2xl border border-slate-700 bg-slate-950 px-4 py-2 font-mono text-xs uppercase tracking-[0.25em] text-slate-300 transition hover:border-slate-600 hover:text-slate-100"
                  >
                    Change key
                  </button>
                </div>
              </div>

              <div className="px-5 py-4">
                {error && loadMode === 'error' ? (
                  <div className="mb-4 rounded-2xl border border-rose-500/40 bg-rose-500/10 px-4 py-3 text-sm text-rose-200">
                    {error}
                  </div>
                ) : null}

                {loadMode === 'loading' ? (
                  <LoadingState />
                ) : sortedEvents.length === 0 ? (
                  <EmptyState />
                ) : (
                  <div className="overflow-hidden rounded-2xl border border-slate-800">
                    <div className="max-h-[65vh] overflow-auto">
                      <table className="min-w-full divide-y divide-slate-800">
                        <thead className="sticky top-0 bg-slate-950/95 backdrop-blur">
                          <tr className="text-left text-xs uppercase tracking-[0.24em] text-slate-500">
                            <th className="px-4 py-3">UID</th>
                            <th className="px-4 py-3">Name</th>
                            <th className="px-4 py-3">Timestamp</th>
                            <th className="px-4 py-3">Received At</th>
                          </tr>
                        </thead>
                        <tbody className="divide-y divide-slate-800 bg-slate-950/60 font-mono text-sm">
                          {sortedEvents.map((event) => (
                            <tr key={`${event.uid}-${event.received_at}`} className="hover:bg-slate-900/70">
                              <td className="whitespace-nowrap px-4 py-3 text-cyan-300">{event.uid}</td>
                              <td className="whitespace-nowrap px-4 py-3 text-slate-100">
                                {event.name?.trim() || 'Unknown'}
                              </td>
                              <td className="whitespace-nowrap px-4 py-3 text-slate-300">
                                {formatDateTime(event.timestamp)}
                              </td>
                              <td className="whitespace-nowrap px-4 py-3 text-slate-300">
                                {formatDateTime(event.received_at)}
                              </td>
                            </tr>
                          ))}
                        </tbody>
                      </table>
                    </div>
                  </div>
                )}
              </div>
            </section>
          </main>
        )}
      </div>
    </div>
  )
}

function SummaryCard({ label, value }: { label: string; value: string }) {
  return (
    <article className="rounded-3xl border border-slate-800 bg-slate-900/80 p-5 shadow-glow">
      <p className="font-mono text-[0.65rem] uppercase tracking-[0.28em] text-slate-500">{label}</p>
      <p className="mt-3 break-words font-mono text-2xl font-semibold text-slate-50">{value}</p>
    </article>
  )
}

function LoadingState() {
  return (
    <div className="space-y-3">
      <div className="flex items-center gap-3 rounded-2xl border border-slate-800 bg-slate-950/70 px-4 py-3">
        <div className="h-4 w-4 animate-spin rounded-full border-2 border-cyan-400 border-t-transparent" />
        <p className="font-mono text-sm text-slate-300">Loading attendance records</p>
      </div>
      <div className="space-y-2">
        {Array.from({ length: 5 }).map((_, index) => (
          <div key={index} className="h-12 animate-pulse rounded-2xl bg-slate-800/80" />
        ))}
      </div>
    </div>
  )
}

function EmptyState() {
  return (
    <div className="flex min-h-[240px] items-center justify-center rounded-2xl border border-dashed border-slate-700 bg-slate-950/50 px-6 text-center">
      <div className="max-w-md">
        <p className="font-mono text-xs uppercase tracking-[0.3em] text-slate-500">
          No events yet
        </p>
        <h3 className="mt-3 text-xl font-semibold text-slate-50">Waiting for attendance data</h3>
        <p className="mt-2 text-sm text-slate-300">
          The backend is reachable, but there are currently no stored attendance events to display.
        </p>
      </div>
    </div>
  )
}
